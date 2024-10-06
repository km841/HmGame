// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "HmPawnExtensionComponent.generated.h"

/**
 * 
 */

// IGameFrameworkInitStateInterface는 GameInstance의 InitState를 편리하게 관리하기 위한 인터페이스
UCLASS()
class HMGAME_API UHmPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UHmPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	static const FName NAME_ActorFeatureName;
	// 컴포넌트나 액터가 생성되는 완전 초반에 일어남
	// 이 단계에서 컴포넌트 부착함
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesireState) const final;
	virtual void CheckDefaultInitialization() final;
};
