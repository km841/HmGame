// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "HmCameraMode.h"
#include "HmCameraComponent.generated.h"

class UHmCameraModeStack;

template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UHmCameraMode>, FHmCameraModeDelegate);
/**
 * 
 */
UCLASS()
class HMGAME_API UHmCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UHmCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UHmCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UHmCameraComponent>() : nullptr); }

	void UpdateCameraModes();

public:
	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;
	

	UPROPERTY()
	TObjectPtr<UHmCameraModeStack> CameraModeStack;

	FHmCameraModeDelegate DetermineCameraModeDelegate;
};
