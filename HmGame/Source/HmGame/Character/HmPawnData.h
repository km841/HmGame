// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HmGame/Input/HmInputConfig.h"
#include "HmPawnData.generated.h"

class UHmCameraMode;
class UHmAbilitySet;
/** 
 * 
 */
UCLASS()
class HMGAME_API UHmPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UHmPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hm|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hm|Camera")
	TSubclassOf<UHmCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hm|InputConfig")
	TObjectPtr<UHmInputConfig> InputConfig;

	// 해당 Pawn의 AbilitySystem에 허용할 AbilitySet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hm|Abilities")
	TArray<TObjectPtr<UHmAbilitySet>> AbilitySets;
};
