// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPlayerCameraManager.h"

AHmPlayerCameraManager::AHmPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = HM_CAMERA_DEFAULT_FOV;
	ViewPitchMin = HM_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = HM_CAMERA_DEFAULT_PITCH_MAX;
}
