// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "HmInputConfig.generated.h"

class UInputAction;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FHmInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


UCLASS(BlueprintType)
class UHmInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UHmInputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FHmInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FHmInputAction> AbilityInputActions;
};