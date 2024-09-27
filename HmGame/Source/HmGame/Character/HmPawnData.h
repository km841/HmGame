// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HmPawnData.generated.h"

/**
 * 
 */
UCLASS()
class HMGAME_API UHmPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UHmPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
