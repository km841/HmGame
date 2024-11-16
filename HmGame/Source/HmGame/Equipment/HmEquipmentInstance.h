// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Array.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "HmEquipmentInstance.generated.h"

struct FHmEquipmentActorToSpawn;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class HMGAME_API UHmEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UHmEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	APawn* GetPawn() const;
	void SpawnEquipmentActors(const TArray<FHmEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();

	// Event Nodes
	// 블루프린트로 Implementable한 이벤트로 만들어줄 것이기 떄문에 따로 구현할 필요가 없음
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

	virtual void OnEquipped();
	virtual void OnUnequipped();

	// 어떤 InventoryItemInstance에 의해 활성화되었는지 (추후 퀵바에서 사용)
	// 해당 장착물을 유발한 자가 누구인가!?
	UPROPERTY()
	TObjectPtr<UObject> Instigator;


	// HmEquipmentDefinition에 맞게 Spawn된 Actor Instance들..
	// 여기서 가지고 있어야 GC가 되지 않는다.
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
	
};
