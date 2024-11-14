#pragma once

#include "UObject/Object.h"
#include "Templates/SubclassOf.h"

#include "HmInventoryItemInstance.generated.h"

class UHmInventoryItemDefinition;

UCLASS(BlueprintType)
class UHmInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UHmInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UHmInventoryItemFragment* FindFragmentByClass(TSubclassOf<UHmInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	/* Inventory Item의 인스턴스에는 무엇으로 정의되었는지 HmInventoryItemDefinition을 들고 있다. */
	UPROPERTY()
	TSubclassOf<UHmInventoryItemDefinition> ItemDef;
};