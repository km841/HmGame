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

	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	// 총탄 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;
};
