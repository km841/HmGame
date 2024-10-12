// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPlayerController.h"
#include "HmGame/Camera/HmPlayerCameraManager.h"

AHmPlayerController::AHmPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AHmPlayerCameraManager::StaticClass();
}
