// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HmMappingConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FHmMappingConfigPair
{
	GENERATED_BODY()

public:
	FHmMappingConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};