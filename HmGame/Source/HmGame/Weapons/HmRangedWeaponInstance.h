// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HmWeaponInstance.h"
#include "HmRangedWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class HMGAME_API UHmRangedWeaponInstance : public UHmWeaponInstance
{
	GENERATED_BODY()
public:
	UHmRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
