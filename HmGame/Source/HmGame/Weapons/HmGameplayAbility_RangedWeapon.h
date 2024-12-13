#pragma once

#include "CoreMinimal.h"
#include "HmGame/Equipment/HmGameplayAbility_FromEquipment.h"
#include "HmGameplayAbility_RangedWeapon.generated.h"

UCLASS()
class UHmGameplayAbility_RangedWeapon : public UHmGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	UHmGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
};