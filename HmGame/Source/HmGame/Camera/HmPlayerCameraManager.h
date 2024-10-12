// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "HmPlayerCameraManager.generated.h"


#define HM_CAMERA_DEFAULT_FOV (80.0f)
#define HM_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define HM_CAMERA_DEFAULT_PITCH_MAX (89.0f)


/**
 * 
 */
UCLASS()
class HMGAME_API AHmPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AHmPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
