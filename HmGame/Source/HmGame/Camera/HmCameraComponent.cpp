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

	// 현재 프레임에서 카메라 모드 뷰 계산
	FHmCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera의 Location과 Rotation을 반영한다.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	FieldOfView = CameraModeView.FieldOfView;

	// 직접적으로 렌더링에 관여하는 파라미터들
	// CameraComponent의 변경 사항을 반영해서 최종 렌더링까지 반영하게 됨
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;

	// 언리얼은 PostProcessing을 Volume으로 세팅하는 방식도 있고, Camera로 세팅하는 방식도 있다.
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}


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