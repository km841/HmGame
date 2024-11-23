#pragma once

#include "AbilitySystemComponent.h"
#include "HmAbilitySystemComponent.generated.h"

UCLASS()
class UHmAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UHmAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};