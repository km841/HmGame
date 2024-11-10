#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HmAnimInstance.generated.h"

UCLASS()
class UHmAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UHmAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1;
};