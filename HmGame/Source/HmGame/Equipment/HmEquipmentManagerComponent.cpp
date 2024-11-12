#include "HmEquipmentManagerComponent.h"
#include "HmEquipmentDefinition.h"
#include "HmEquipmentInstance.h"

UHmEquipmentManagerComponent::UHmEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
}
