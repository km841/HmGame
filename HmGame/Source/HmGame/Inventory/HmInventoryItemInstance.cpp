#include "HmInventoryItemInstance.h"

UHmInventoryItemInstance::UHmInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const UHmInventoryItemFragment* UHmInventoryItemInstance::FindFragmentByClass(TSubclassOf<UHmInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		// HmInventoryItemDefinition은 모든 멤버변수가 EditDefaultsOnly로 선언되어 있으므로, GetDefault로 가져와도 무방
		// - Fragment 정보는 Instance가 아닌 Definition에 있다.
		return GetDefault<UHmInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
