#include "HmAbilitySet.h"
#include "HmAbilitySystemComponent.h"
#include "Abilities/HmGameplayAbility.h"

UHmAbilitySet::UHmAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHmAbilitySet::GiveToAbilitySystem(UHmAbilitySystemComponent* HmASC, FHmAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(HmASC);

	if (!HmASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FHmAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		// CDO로 전달하는 이유는 Ability의 Instance 개수를 최소화하기 위해서
		UHmGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UHmGameplayAbility>();

		// AbilitySpec은 GiveAbility로 전달됨
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = HmASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}

void FHmAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FHmAbilitySet_GrantedHandles::TakeFromAbilitySystem(UHmAbilitySystemComponent* HmASC)
{
	if (!HmASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			HmASC->ClearAbility(Handle);
		}
	}
}

