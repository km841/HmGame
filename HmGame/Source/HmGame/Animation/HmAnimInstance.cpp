#include "HmAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"

UHmAnimInstance::UHmAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHmAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UHmAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC 내부적으로 관리하는 GameplayTag와 AnimInstance의 멤버 Property와 Delegate를 연결하여, 각 변화에 대한 반영을 진행한다.
	GameplayTagPropertyMap.Initialize(this, ASC);
}
