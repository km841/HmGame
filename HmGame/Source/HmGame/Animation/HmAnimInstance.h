#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "HmAnimInstance.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UHmAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UHmAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeInitializeAnimation() override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1;

	UPROPERTY(EditDefaultsOnly, Category="GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};