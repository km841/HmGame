#pragma once

#include "HmCharacterPartTypes.h"
#include "HmCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "HmPawnComponent_CharacterParts.generated.h"

class UHmPawnComponent_CharacterParts;

USTRUCT()
struct FHmAppliedCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part의 정의(메타 데이터)
	UPROPERTY()
	FHmCharacterPart Part;

	// HmCharacterPartList에서 할당 받은 Part 핸들값 (FHmControllerCharacterPartEntry의 Handle 값과 같아야 함 -> 같으면 같은 Part)
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FHmCharacterPartList
{
	GENERATED_BODY()

	FHmCharacterPartList()
		:OwnerComponent(nullptr)
	{}

	FHmCharacterPartList(UHmPawnComponent_CharacterParts* InOwnerComponent)
		:OwnerComponent(InOwnerComponent)
	{}

	FGameplayTagContainer CollectCombinedTags() const;

	bool SpawnActorForEntry(FHmAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FHmAppliedCharacterPartEntry& Entry);

	FHmCharacterPartHandle AddEntry(FHmCharacterPart NewPart);
	void RemoveEntry(FHmCharacterPartHandle Handle);

	// 현재 인스턴스화된 Character Parts
	UPROPERTY()
	TArray<FHmAppliedCharacterPartEntry> Entries;

	UPROPERTY()
	TObjectPtr<UHmPawnComponent_CharacterParts> OwnerComponent;

	int32 PartHandleCounter = 0;

};

UCLASS(meta = (BlueprintSpawnableComponent))
class UHmPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	UHmPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();
	FHmCharacterPartHandle AddCharacterPart(const FHmCharacterPart& NewPart);
	void RemoveCharacterPart(FHmCharacterPartHandle Handle);

	UPROPERTY()
	FHmCharacterPartList CharacterPartList;

	UPROPERTY(EditAnywhere, Category=Cosmetics)
	FHmAnimBodyStyleSelectionSet BodyMeshes;
};