#include "HmGameplayAbility_FromEquipment.h"

UHmGameplayAbility_FromEquipment::UHmGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

UHmEquipmentInstance* UHmGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	// CurrentActorInfo의 AbilitySystemComponent와 CurrentSpecHandle을 활용하여, GameplayAbilitySpec을 가져옴
	// - CurrentSpecHandle은 SetCurrentActorInfo() 호출할 때, Handle 값을 받아서 저장됨
	// - CurrentSpecHandle과 CurrentActorInfo는 같이 함
	// - FindAbilitySpecFromHandle은 GiveAbility로 허용된 ActivitableAbilities를 순회하여 GameplayAbilitySpec을 찾아냄

	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		// GameplayAbility_FromEquipment는 EquipmentInstance로부터 GiveAbility를 진행했으므로, SourceObject에 EquipmentInstance가 저장되어 있음
		return Cast<UHmEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}
