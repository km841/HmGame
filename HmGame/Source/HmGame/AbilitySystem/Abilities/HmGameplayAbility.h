#pragma once

#include "Abilities/GameplayAbility.h"
#include "HmGameplayAbility.generated.h"

UCLASS(Abstract)
class UHmGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UHmGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};