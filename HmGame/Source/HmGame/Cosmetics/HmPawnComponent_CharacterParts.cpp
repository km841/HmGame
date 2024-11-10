#include "HmPawnComponent_CharacterParts.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

UHmPawnComponent_CharacterParts::UHmPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CharacterPartList(this)
{

}

USkeletalMeshComponent* UHmPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	// Character를 활용하여 최상위 Skeletal Mesh 반환
	if (AActor* OwnerActor = GetOwner())
	{
		// Character에서 Mesh를 캐싱해놓고 있다.
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
			{
				return MeshComponent;
			}
		}
	}
	return nullptr;
}

USceneComponent* UHmPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	// Parent에 SkeletalMesh가 있으면 반환
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}

	// 없으면 RootComponent 확인
	else if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}

	// 둘다 없으면 nullptr
	return nullptr;
}

FGameplayTagContainer UHmPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	// 현재 장착된 CharacterPartList의 Merge된 Tags를 반환한다.
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		// 만약 GameplayTag를 통해 필터링될 경우, 필터링해서 진행한다.
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}

	else
	{
		// 필터링할 게 없으면 그냥 반환
		return Result;
	}
}

void UHmPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPos = true;
	
	// 현재 Owner의 SkeletalMesh 반환
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// BodyMeshes를 통해 GameplayTag를 활용하여 알맞은 SkeletalMesh로 재설정!
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

		// SkeletalMesh 초기화 및 Animation 초기화
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPos);

		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPos);
		}
	}
}

FHmCharacterPartHandle UHmPawnComponent_CharacterParts::AddCharacterPart(const FHmCharacterPart& NewPart)
{

	return CharacterPartList.AddEntry(NewPart);
}

void UHmPawnComponent_CharacterParts::RemoveCharacterPart(FHmCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}

FGameplayTagContainer FHmCharacterPartList::CollectCombinedTags() const
{

	FGameplayTagContainer Result;

	for (const FHmAppliedCharacterPartEntry& Entry : Entries)
	{
		if (Entry.SpawnedComponent)
		{
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(Result);
			}
		}
	}

	return Result;
}

bool FHmCharacterPartList::SpawnActorForEntry(FHmAppliedCharacterPartEntry& Entry)
{
	bool bCreateAnyActor = false;
	if (Entry.Part.PartClass != nullptr)
	{
		// OwnerComponent의 Owner에 속한 World 반환
		UWorld* World = OwnerComponent->GetWorld();

		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			// 우리는 Actor - Actor 결합이므로 ChildActorComponent 활용
			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
			PartComponent->SetChildActorClass(Entry.Part.PartClass);

			// RegisterComponent를 통해 마지막으로 RenderWorld인 FScene에 변경 내용을 전달한다.
			PartComponent->RegisterComponent();

			// ChildActorComponent에서 생성된 Actor를 반환
			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				// 해당 Actor가 Parent인 HmPawnComponent_CharacterParts의 Owner Actor보다 먼저 Tick이 실행되지 않도록 선행조건을 붙인다.
				if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
				{
					SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
				}
			}
			Entry.SpawnedComponent = PartComponent;
			bCreateAnyActor = true;
		}
	}

	return bCreateAnyActor;
}

void FHmCharacterPartList::DestroyActorForEntry(FHmAppliedCharacterPartEntry& Entry)
{
	// 바로 제거되진 않고 GC에게 마킹해줌
	if (Entry.SpawnedComponent)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

FHmCharacterPartHandle FHmCharacterPartList::AddEntry(FHmCharacterPart NewPart)
{
	FHmCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FHmAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;

		if (SpawnActorForEntry(NewEntry))
		{
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}

void FHmCharacterPartList::RemoveEntry(FHmCharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FHmAppliedCharacterPartEntry& Entry = *EntryIt;

		if (Entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(Entry);
		}
	}
}
