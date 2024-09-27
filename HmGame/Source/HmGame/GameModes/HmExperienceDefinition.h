// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HmExperienceDefinition.generated.h"

class UHmPawnData;

UCLASS()
class HMGAME_API UHmExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UHmExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<UHmPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;
};