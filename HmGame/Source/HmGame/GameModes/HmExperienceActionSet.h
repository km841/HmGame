// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HmExperienceActionSet.generated.h"

/**
 * 
 */

class UGameFeatureAction;

UCLASS()
class HMGAME_API UHmExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UHmExperienceActionSet();

	UPROPERTY(EditAnywhere, Category="Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
	
};
