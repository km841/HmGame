// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HmCameraMode.generated.h"

class UHmCameraComponent;

UENUM()
enum class EHmCameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

/**
 * 
 */

struct FHmCameraModeView
{
	FHmCameraModeView();

	void Blend(const FHmCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

UCLASS()
class UHmCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UHmCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UHmCameraComponent* GetHmCameraComponent() const;
	AActor* GetTargetActor() const;

	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	// Third Person View에서 해당 함수를 Override에서 사용하기 때문에 원래 Lyra에서는 가상함수로 되어 있다!
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	void UpdateCameraMode(float DeltaTime);

	// Update에서 항상 Update해준다.
	FHmCameraModeView View;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin= "5.0", UIMax="170", ClampMin="5.0", ClampMax="170"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin="-89.9", UIMax="89.9", ClampMin="-89.9", ClampMax="89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	// 블렌딩 진행 시간
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// 선형적인 Blend 값(알파블렌딩)
	float BlendAlpha;


	// 해당 CameraBlend의 최종 Blend 값
	float BlendWeight;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendExponent;

	EHmCameraModeBlendFunction BlendFunction;
};


UCLASS()
class UHmCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UHmCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 없으면 캐싱해놓고, 있으면 캐싱되어있는 카메라 모드 인스턴스를 반환
	// 타입 당 하나만 보관하는 기법
	UHmCameraMode* GetCameraModeInstance(TSubclassOf<UHmCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<UHmCameraMode>& CameraModeClass);

	void UpdateStack(float DeltaTime);
	void BlendStack(FHmCameraModeView& OutCameraModeView);

	void EvaluateStack(float DeltaTime, FHmCameraModeView& OutCameraModeView);

	UPROPERTY()
	TArray<TObjectPtr<UHmCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UHmCameraMode>> CameraModeStack;
};

