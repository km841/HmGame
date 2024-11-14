#include "HmInventoryItemDefinition.h"

UHmInventoryItemDefinition::UHmInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const UHmInventoryItemFragment* UHmInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UHmInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		// Fragments를 순회하며, IsA를 통해 해당 클래스를 가지고 있는지 확인한다.
		for (UHmInventoryItemFragment* Fragment : Fragments)
		{
			// IsA는 해당 클래스를 상속받고 있는지를 검사하는 함수
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
