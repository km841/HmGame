// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPlayerController.h"
#include "HmGame/Camera/HmPlayerCameraManager.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"
#include "HmPlayerState.h"

AHmPlayerController::AHmPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AHmPlayerCameraManager::StaticClass();
}

void AHmPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UHmAbilitySystemComponent* HmASC = GetHmAbilitySystemComponent())
	{
		HmASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

AHmPlayerState* AHmPlayerController::GetHmPlayerState() const
{
	return CastChecked<AHmPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UHmAbilitySystemComponent* AHmPlayerController::GetHmAbilitySystemComponent() const
{
	const AHmPlayerState* HmPS = GetHmPlayerState();
	return (HmPS ? HmPS->GetHmAbilitySystemComponent() : nullptr);
}
