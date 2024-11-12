#include "HmInventoryManagerComponent.h"

UHmInventoryManagerComponent::UHmInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}
