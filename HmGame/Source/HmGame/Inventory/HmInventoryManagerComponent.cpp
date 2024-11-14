#include "HmInventoryManagerComponent.h"
#include "HmInventoryItemDefinition.h"
#include "HmInventoryItemInstance.h"

UHmInventoryItemInstance* FHmInventoryList::AddEntry(TSubclassOf<UHmInventoryItemDefinition> ItemDef)
{
	UHmInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);
	
	// Owner를 가져온다 (B_ShooterGame_Mannequin)
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	// Object를 만들 때 Owner를 Outer로 넣어준다
	FHmInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UHmInventoryItemInstance>(OwningActor);
	
	// Entry는 Instance를 소유하고, Instance의 멤버로 Definition을 소유 
	NewEntry.Instance->ItemDef = ItemDef;

	Result = NewEntry.Instance;
	return Result;
}


UHmInventoryManagerComponent::UHmInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}

UHmInventoryItemInstance* UHmInventoryManagerComponent::AddItemDefinition(TSubclassOf<UHmInventoryItemDefinition> ItemDef)
{
	UHmInventoryItemInstance* Result = nullptr;

	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}

	return Result;
}
