// Fill out your copyright notice in the Description page of Project Settings.


#include "HmWeaponInstance.h"

UHmWeaponInstance::UHmWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

TSubclassOf<UAnimInstance> UHmWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FHmAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
