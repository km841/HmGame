#pragma once

#include "Components/ControllerComponent.h"
#include "HmQuickBarComponent.generated.h"

class UHmInventoryItemInstance;
class UHmEquipmentInstance;


// HUD의 QuickBar를 생각하면 된다
// - 흔히 MMORPG의 ShortCut HUD를 연상하면 됨

// 해당 Component는 ControllerComponent로써 , PlayerController에 의해 조작계 중 하나라고 생각하면 됨
// HUD(Slate)와 Inventory/Equipment(Gameplay) 의 다리(Bridge)역할을 하는 Component로 생각하자
// 해당 Component는 Lyra의 HUD 및 Slate Widget을 다루면서 다시 볼 예정
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UHmQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UHmQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// HUD Quick Bar Slot 개수
	UPROPERTY()
	int32 NumSlots = 3;

	// HUD Quick Bar Slot 리스트
	TArray<TObjectPtr<UHmInventoryItemInstance>> Slots;

	// 현재 활성화된 Slot 인덱스 (아마 Lyra는 한 개의 슬롯이 활성화되는 듯)
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	// 현재 장착된 장비 정보
	UPROPERTY()
	TObjectPtr<UHmEquipmentInstance> EquippedItem;

};