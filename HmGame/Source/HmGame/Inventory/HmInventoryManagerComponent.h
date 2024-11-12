#pragma once

#include "Components/ActorComponent.h"
#include "HmInventoryManagerComponent.generated.h"

class UHmInventoryItemInstance;

USTRUCT(BlueprintType)
struct FHmInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UHmInventoryItemInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FHmInventoryList
{
	GENERATED_BODY()

	FHmInventoryList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	UPROPERTY()
	TArray<FHmInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

UCLASS(BlueprintType)
class UHmInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UHmInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FHmInventoryList InventoryList;
};