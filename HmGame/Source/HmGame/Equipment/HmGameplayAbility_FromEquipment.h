#pragma once

#include "CoreMinimal.h"
#include "HmGame/AbilitySystem/Abilities/HmGameplayAbility.h"
#include "HmGameplayAbility_FromEquipment.generated.h"

class UHmEquipmentInstance;

UCLASS()
class UHmGameplayAbility_FromEquipment : public UHmGameplayAbility
{
	GENERATED_BODY()
public:
	UHmGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UHmEquipmentInstance* GetAssociatedEquipment() const;
};