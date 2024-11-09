#pragma once

#include "GameplayTagContainer.h"
#include "HmCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FHmAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	// AnimLayer를 적용할 대상 Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	// Cosmetic Tag라고 생각하면 됨
	// 레벨 제한이나, 스텟 제한으로 인해 이 파츠는 낄수 없다! 라는 개념으로 보면 됨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FHmAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	// GameplayTag를 통해 (CosmeticTags), Mesh Rules에 따라 알맞은 BodyStyle을 반환한다!
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

	// AnimLayer를 적용할 SkeletalMesh를 들고 있음, Animation->Mesh 간 Rules를 MeshRules라고 생각하면 됨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHmAnimBodyStyleSelectionEntry> MeshRules;

	// 그냥 디폴트로 적용할 Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// Physics Asset은 하나로 통일함 -> 즉 모든 Animation의 Physics 속성은 공유한다.
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};