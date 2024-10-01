// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HmGameState.generated.h"

class UHmExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class HMGAME_API AHmGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHmGameState();

public:
	UPROPERTY()
	TObjectPtr<UHmExperienceManagerComponent> ExperienceManagerComponent;
	
};
