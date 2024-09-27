// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HmUserFacingExperience.generated.h"

UCLASS()
class HMGAME_API UHmUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Map의 정보
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapId;

	// AllowedTypes는 해당 개체만 이 변수에 넣을 수 있다!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "HmExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
