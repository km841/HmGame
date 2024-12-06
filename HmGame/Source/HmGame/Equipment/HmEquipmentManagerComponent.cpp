#include "HmEquipmentManagerComponent.h"
#include "HmEquipmentDefinition.h"
#include "HmEquipmentInstance.h"
#include "AbilitySystemGlobals.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"
#include "HmGame/AbilitySystem/HmAbilitySet.h"

UHmEquipmentInstance* FHmEquipmentList::AddEntry(TSubclassOf<UHmEquipmentDefinition> EquipmentDefinition)
{
	UHmEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	// EquipmentDefinition의 멤버 변수들은 EditDefaultsOnly로 정의되어 있어 GetDefaults로 들고 와도 우리에게 필요한 것들만 들어 있다.
	const UHmEquipmentDefinition* EquipmentCDO = GetDefault<UHmEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UHmEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UHmEquipmentInstance::StaticClass();
	}

	// Entries에 추가
	FHmAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UHmEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	UHmAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	{
		for (TObjectPtr<UHmAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

	// ActorsToSpawn을 통해 Actor들을 인스턴스화 한다
	// 어디에 ? EquipmentInstance에!
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FHmEquipmentList::RemoveEntry(UHmEquipmentInstance* Instance)
{
	// 단순히 Instance를 찾아서 순회
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FHmAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			UHmAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			// Actor 제거 및 Iterator에서 안전하게 제거
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UHmAbilitySystemComponent* FHmEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();

	// - EquipmentManagerComponent는 AHmCharacter를 Owner로 가지고 있다
	// - 해당 함수는 IAbilitySystemInterface를 통해 AbilitySystemComponent를 반환한다
	// - 우리는 ACharacter에 IAbilitySystemInterface를 상속받아야 한다!
	return Cast<UHmAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UHmEquipmentManagerComponent::UHmEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
}

UHmEquipmentInstance* UHmEquipmentManagerComponent::EquipItem(TSubclassOf<UHmEquipmentDefinition> EquipmentDefinition)
{
	UHmEquipmentInstance* Result= nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}

	return Result;
}

void UHmEquipmentManagerComponent::UnequipItem(UHmEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		//BP의 이벤트 노드를 호출시키는 함수	
		ItemInstance->OnUnequipped();

		EquipmentList.RemoveEntry(ItemInstance);
	}
}


