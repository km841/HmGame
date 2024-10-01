// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HmGameModeBase.generated.h"

/**
 * 
 */

class UHmPawnData;
class UHmExperienceDefinition;

UCLASS()
class HMGAME_API AHmGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHmGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() final;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;


	void HandleMatchAssignmentIfNotExceptingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);

	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const UHmExperienceDefinition* CurrentExperience);

	const UHmPawnData* GetPawnDataForController(const AController* InController) const;

};
