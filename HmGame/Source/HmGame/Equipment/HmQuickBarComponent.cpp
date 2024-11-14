#include "HmQuickBarComponent.h"
#include "HmEquipmentManagerComponent.h"
#include "HmEquipmentDefinition.h"
#include "HmEquipmentInstance.h"
#include "HmGame/Inventory/HmInventoryItemInstance.h"
#include "HmGame/Inventory/HmInventoryFragment_EquippableItem.h"

UHmQuickBarComponent::UHmQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHmQuickBarComponent::BeginPlay()
{
	// NumSlots에 따라 미리 할당
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

UHmEquipmentManagerComponent* UHmQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UHmEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UHmQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	// 활성화된 InventoryItemInstance를 찾는다!
	if (UHmInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// Slot Item을 찾아서 InventoryFragment_EquppableItem의 Fragment를 찾는다.
		// 찾는 것이 실패했다면, 장착할 수 없는 Inventory Item임을 의미한다.
		if (const UHmInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UHmInventoryFragment_EquippableItem>())
		{

			// EquipmentItem에서 EquipmentDefinition 찾기
			// EquipDef가 있어야 장착 가능하다
			TSubclassOf<UHmEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{

				// Unequip과 비슷하게 EquipmentManager를 통해 장착
				if (UHmEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						// 누가 나에게 아이템을 장착하도록 시켰지?
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}

void UHmQuickBarComponent::UnequipItemInSlot()
{
	// *QuickBar는 Controller Component이지만, EquipmentManager는 Controller가 소유(Possess)하고 있는 Pawn의 Component이다!
	if (UHmEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		// 현재 장착된 Item이 있다면,
		if (EquippedItem)
		{
			// EquipmentManager를 통해 Pawn의 장비를 해제시킨다.
			EquipmentManager->UnequipItem(EquippedItem);

			// 그리고 Controller에서도 EquippedItem 상태를 없는 걸로 인식
			EquippedItem = nullptr;
		}
	}
}

void UHmQuickBarComponent::AddItemToSlot(int32 SlotIndex, UHmInventoryItemInstance* Item)
{
	// Add로 동적 추가가 아닌 Index로 바로 밀어넣는다!!
	// - 그럼 Resize가 미리 되었어야 하는데, 그건 BeginPlay에서 진행

	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UHmQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		// 뭔가 업데이트가 되면 Unequip -> Update -> Equip 방식으로 업데이트를 진행해준다!
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}
