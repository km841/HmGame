// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HmEquipmentInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class HMGAME_API UHmEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UHmEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
