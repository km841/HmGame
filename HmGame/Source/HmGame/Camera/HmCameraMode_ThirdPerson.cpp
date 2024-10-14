// Fill out your copyright notice in the Description page of Project Settings.


#include "HmCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"

UHmCameraMode_ThirdPerson::UHmCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHmCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
