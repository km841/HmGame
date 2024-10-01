// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HmPlayerState.generated.h"


class UHmPawnData;
class UHmExperienceDefinition;

/**
 * 
 */
UCLASS()
class HMGAME_API AHmPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializerComponents() final;

	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void OnExperienceLoaded(const UHmExperienceDefinition* CurrentExperience);
	void SetPawnData(const UHmPawnData* InPawnData);

	UPROPERTY()
	TObjectPtr<const UHmPawnData> PawnData;
};
