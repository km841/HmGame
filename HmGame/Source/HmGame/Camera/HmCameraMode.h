// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HmCameraMode.generated.h"

/**
 * 
 */
UCLASS()
class UHmCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UHmCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};


UCLASS()
class UHmCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UHmCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TArray<TObjectPtr<UHmCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UHmCameraMode>> CameraModeStack;


};

