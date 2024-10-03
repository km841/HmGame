// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HmGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HMGAME_API UHmGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	virtual void Init() override;
	virtual void Shutdown() override;
};
