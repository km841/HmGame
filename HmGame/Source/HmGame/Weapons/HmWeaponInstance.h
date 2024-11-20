// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HmGame/Equipment/HmEquipmentInstance.h"
#include "HmGame/Cosmetics/HmCosmeticAnimationTypes.h"
#include "HmWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class HMGAME_API UHmWeaponInstance : public UHmEquipmentInstance
{
	GENERATED_BODY()
public:
	UHmWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	// Weapon에 Equip / Unequip에 대한 Animation Set 정보를 들고 있다. 
	// 무기 단위로 AnimLayer를 들고 있다?
	// 경찰 특공대처럼 한쪽 손에는 권총, 한쪽 손에는 방패를 들고 있다면 
	// 그 것을 하나의 Set 단위로 묶어서 애니메이션이 나와줘야 한다!
	// 따라서 WeaponInstance가 AnimLayer를 들고 있는 것.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FHmAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FHmAnimLayerSelectionSet UnequippedAnimSet;
	
};
