#pragma once

#include "Abilities/GameplayAbility.h"
#include "HmGameplayAbility.generated.h"

UENUM()
enum class EHmAbilityActivationPolicy : uint8
{
	// Input이 트리거되었을 경우 (Pressed / Released)
	OnInputTriggered,

	// Input이 Held되어 있을 경우
	WhileInputActive,

	// Avatar가 생성되었을 경우 바로 할당
	OnSpawn,
};

UCLASS(Abstract)
class UHmGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UHmGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hm|AbilityActivation")
	EHmAbilityActivationPolicy ActivationPolicy;
};