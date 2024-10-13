// Fill out your copyright notice in the Description page of Project Settings.


#include "HmCameraComponent.h"
#include "HmCameraMode.h"

UHmCameraComponent::UHmCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CameraModeStack(nullptr)
{

}

void UHmCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UHmCameraModeStack>(this);
	}

}

void UHmCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);
	UpdateCameraModes();
}

void UHmCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (DetermineCameraModeDelegate.IsBound())
	{
		// 현재 플레이어가 사용하는 카메라 모드를 매 프레임 저장한다.
		// 왜 매 프레임 저장하는가?
		if (TSubclassOf<UHmCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}