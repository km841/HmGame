// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HmCameraMode.h"
#include "HmCameraMode_ThirdPerson.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class HMGAME_API UHmCameraMode_ThirdPerson : public UHmCameraMode
{
	GENERATED_BODY()

public:
	UHmCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;

};
