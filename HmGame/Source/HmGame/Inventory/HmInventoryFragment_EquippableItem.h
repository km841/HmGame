#pragma once

#include "HmInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "HmInventoryFragment_EquippableItem.generated.h"

class UHmEquipmentDefinition;

UCLASS()
class UHmInventoryFragment_EquippableItem : public UHmInventoryItemFragment
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = Hm)
	TSubclassOf<UHmEquipmentDefinition> EquipmentDefinition;
};