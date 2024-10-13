// Fill out your copyright notice in the Description page of Project Settings.


#include "HmCameraMode.h"
#include "HmCameraComponent.h"
#include "HmPlayerCameraManager.h"

FHmCameraModeView::FHmCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(HM_CAMERA_DEFAULT_FOV)
{
}

UHmCameraMode::UHmCameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView = HM_CAMERA_DEFAULT_FOV;
	ViewPitchMin = HM_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = HM_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = EHmCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

UHmCameraComponent* UHmCameraMode::GetHmCameraComponent() const
{
	return CastChecked<UHmCameraComponent>(GetOuter());
}

AActor* UHmCameraMode::GetTargetActor() const
{
	const UHmCameraComponent* HmCameraComponent = GetHmCameraComponent();
	return HmCameraComponent->GetTargetActor();
}

FVector UHmCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UHmCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRotation() 확인, 보통 Pawn의 ControlRotation 반환
		return TargetPawn->GetViewRotation();
	}


	return TargetActor->GetActorRotation();
}

void UHmCameraMode::UpdateView(float DeltaTime)
{
	// Pivot이란? 카메라를 회전시키는 회전 축
	// 카메라는 플레이어를 보고있어야 하므로 자신의 로컬 좌표를 중심으로 돌 일이 없기 때문
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch는 바라보는 방향을 기준으로 가로 축(X축)이라고 보면 됨
	// X축을 기준으로 회전하니 위, 아래로 회전하는 것
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

}

void UHmCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;

	switch (BlendFunction)
	{
		case EHmCameraModeBlendFunction::Linear:
			BlendWeight = BlendAlpha;
			break;

		case EHmCameraModeBlendFunction::EaseIn:
			BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
			break;

		case EHmCameraModeBlendFunction::EaseOut:
			BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
			break;
		case EHmCameraModeBlendFunction::EaseInOut:
			BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
			break;

		default:
			checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
			break;
	}
}

void UHmCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

UHmCameraModeStack::UHmCameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UHmCameraMode* UHmCameraModeStack::GetCameraModeInstance(TSubclassOf<UHmCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	for (UHmCameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	UHmCameraMode* NewCameraMode = NewObject<UHmCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);
	return NewCameraMode;
}

void UHmCameraModeStack::PushCameraMode(TSubclassOf<UHmCameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	UHmCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// 가장 최근에 쌓인 요소이므로 그냥 리턴!
		return;
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}

		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}


	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	CameraModeStack.Insert(CameraMode, 0);
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UHmCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UHmCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UHmCameraModeStack::BlendStack(FHmCameraModeView& OutCameraModeView)
{
}

void UHmCameraModeStack::EvaluateStack(float DeltaTime, FHmCameraModeView& OutCameraModeView)
{
	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);
}


