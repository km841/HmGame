#pragma once

#include "Components/PawnComponent.h"
#include "HmPawnComponent_CharacterParts.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class UHmPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	UHmPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};