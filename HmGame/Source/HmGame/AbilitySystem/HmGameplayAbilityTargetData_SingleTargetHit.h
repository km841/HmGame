#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "HmGameplayAbilityTargetData_SingleTargetHit.generated.h"

USTRUCT()
struct FHmGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FHmGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FHmGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	int32 CartridgeID;
};

