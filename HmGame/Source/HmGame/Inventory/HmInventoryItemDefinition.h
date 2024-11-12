#pragma once

#include "CoreMinimal.h"
#include "HmInventoryItemDefinition.generated.h"

/*
* Inventory에 대한 Fragment (잘 이해되지 않을 수 있는 주제)
* - Lyra에서 사용되는 예시를 통한 이해
*   - ULyraInventoryFragment_EquippableItem은 EquipmentItemDefinition을 가지고 있으며, 장착 가능한 아이템을 의미한다.
*   - ULyraInventoryFragment_SetStatus는 아이템에 대한 정보를 가지고 있다.
*     - Rifle에 대한 Status로는 총알(Ammo)에 대한 장착 최대치와 현재 남은 잔탄 수를 예시로 들 수 있다.
*   - 등등...
*/

UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class UHmInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:

};


// 아이템의 메타데이터로써 어떠한 정의를 가지고 있는가를 표현하는 클래스
UCLASS(Blueprintable)
class UHmInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UHmInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// InventoryItem 정의(메타) 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	// Weapon의 Component 개념이라고 생각하면 됨
	// Actor - Component라는 명칭은 중복이라 사용할 수 없으니
	// Fragment라는 명칭을 이용해서 관계를 정의
	// 총을 예로 들면 Ammo와 같은 값들을 Status Component에 관리하는 것.
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category=Display)
	TArray<TObjectPtr<UHmInventoryItemFragment>> Fragments;
};