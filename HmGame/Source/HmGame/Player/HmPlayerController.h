// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "HmPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HMGAME_API AHmPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	AHmPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
