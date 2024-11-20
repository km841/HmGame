// Fill out your copyright notice in the Description page of Project Settings.


#include "HmEquipmentInstance.h"
#include "HmEquipmentDefinition.h"
#include "GameFramework/Character.h"

UHmEquipmentInstance::UHmEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

APawn* UHmEquipmentInstance::GetPawn() const
{
	// 우리는 EquipmentInstance를 생성 시 Outer를 Pawn으로 두었다.
	return Cast<APawn>(GetOuter());
}

APawn* UHmEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}

	return Result;
}

void UHmEquipmentInstance::SpawnEquipmentActors(const TArray<FHmEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		// 현재 Owner인 Pawn의 RootComponent를 AttachTarget의 대상으로 한다.
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			// B_Hero_Mannequin의 Invisible Skeletal Mesh에 붙게 된다!
			AttachTarget = Char->GetMesh();
		}

		for (const FHmEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			// SpawnActorDeffered는 FinishingSpawn과 함께 사용해야 완성된다. (작성자에게 코드로써 Ownership이 있다는 것)
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);

			// Actor의 RelativeTransform을 AttachTransform으로 설정
			NewActor->SetActorRelativeTransform(SpawnInfo.AttackTransform);

			// AttachActor에게 붙이기! (Actor -> Actor)
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UHmEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UHmEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UHmEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
