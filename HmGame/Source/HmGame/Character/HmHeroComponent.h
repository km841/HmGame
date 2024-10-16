// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "HmHeroComponent.generated.h"

class UHmCameraMode;
/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class HMGAME_API UHmHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UHmHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;


	static const FName NAME_ActorFeatureName;

	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;

	// 현 상태에서 다음 상태로 넘어갈 때 그게 가능한 지 확인하는 함수
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState);
	virtual void CheckDefaultInitialization() final;

	
	TSubclassOf<UHmCameraMode> DetermineCameraMode() const;

};
