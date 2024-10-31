// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "HmExperienceManagerComponent.generated.h"

/**
 * 
 */

class UHmExperienceDefinition;

enum class EHmExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHmExperienceLoaded, const UHmExperienceDefinition*);

UCLASS()
class HMGAME_API UHmExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	bool IsExperienceLoaded() { return (LoadState == EHmExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	void CallOrRegister_OnExperienceLoaded(FOnHmExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();

	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);

	void OnExperienceFullLoadCompleted();

	const UHmExperienceDefinition* GetCurrentExperienceChecked() const;

public:
	UPROPERTY()
	TObjectPtr<const UHmExperienceDefinition> CurrentExperience;

	EHmExperienceLoadState LoadState = EHmExperienceLoadState::Unloaded;

	FOnHmExperienceLoaded OnExperienceLoaded;

	int32 NumGameFeaturePluginLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
	
};
