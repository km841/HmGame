#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "HmAbilitySet.generated.h"

class UHmGameplayAbility;
class UHmAbilitySystemComponent;


USTRUCT(BlueprintType)
struct FHmAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	// 허용된 GameplayAbility
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHmGameplayAbility> Ability = nullptr;

	// Input 처리를 위한 GameplayTag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	// Ability의 허용 조건
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FHmAbilitySet_GrantedHandles
{
	GENERATED_BODY()
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(UHmAbilitySystemComponent* HmASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};


UCLASS(BlueprintType)
class UHmAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UHmAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UHmAbilitySystemComponent* HmASC, FHmAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FHmAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};