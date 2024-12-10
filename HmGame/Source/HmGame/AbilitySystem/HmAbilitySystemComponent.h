#pragma once

#include "AbilitySystemComponent.h"
#include "HmAbilitySystemComponent.generated.h"

UCLASS()
class UHmAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UHmAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	// Ability Input 처리할 Pending Queue
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};