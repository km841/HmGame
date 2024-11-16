#pragma once
#include "Components/PawnComponent.h"
#include "HmEquipmentManagerComponent.generated.h"

class UHmEquipmentDefinition;
class UHmEquipmentInstance;

USTRUCT(BlueprintType)
struct FHmAppliedEquipmentEntry
{
	GENERATED_BODY()

	// 장착물에 대한 메타 데이터
	UPROPERTY()
	TSubclassOf<UHmEquipmentDefinition> EquipmentDefinition;

	// EquipmentDefinition을 통해 생성된 인스턴스
	UPROPERTY()
	TObjectPtr<UHmEquipmentInstance> Instance = nullptr;
};

// EquipmentInstance의 Instance를 Entry에서 관리하고 있다.
// 해당 클래스는 생성된 객체를 관리한다고 보면 됨
USTRUCT(BlueprintType)
struct FHmEquipmentList
{
	GENERATED_BODY()

	FHmEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	UHmEquipmentInstance* AddEntry(TSubclassOf<UHmEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UHmEquipmentInstance* Instance);

	// 장착물에 대한 관리 리스트
	UPROPERTY()
	TArray<FHmAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

// 갑옷이나 이런건 Pawn에 붙여야 하므로 PawnComponent를 상속받는 것.
UCLASS(BlueprintType)
class UHmEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UHmEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UHmEquipmentInstance* EquipItem(TSubclassOf<UHmEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(UHmEquipmentInstance* ItemInstance);

	UPROPERTY()
	FHmEquipmentList EquipmentList;
};