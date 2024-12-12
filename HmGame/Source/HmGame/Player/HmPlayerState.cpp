// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPlayerState.h"
#include "HmGame/GameModes/HmExperienceManagerComponent.h"
#include "HmGame/GameModes/HmGameModeBase.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"
#include "HmGame/AbilitySystem/HmAbilitySet.h"
#include "HmGame/Character/HmPawnData.h"

PRAGMA_DISABLE_OPTIMIZATION
AHmPlayerState::AHmPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UHmAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}
PRAGMA_ENABLE_OPTIMIZATION

void AHmPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	// 아래의 코드는 우리가 InitAbilityActorInfo를 재호출을 통하는 이유를 설명하는 코드이다:
	{
		// 처음 InitAbilityActorInfo를 호출 당시, OwnerActor와 AvatarActo가 같은 Actor를 가르키고 있으며, 이는 PlayerState이다
		// - OwnerActor는 PlayerState가 의도하는게 맞지만, AvatarActor는 PlayerController가 소유하는 대상인 Pawn이 되어야 한다!
		// - 이를 위해 재-세팅을 해준다
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UHmExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UHmExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnHmExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AHmPlayerState::OnExperienceLoaded(const UHmExperienceDefinition* CurrentExperience)
{
	if (AHmGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AHmGameModeBase>())
	{
		const UHmPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void AHmPlayerState::SetPawnData(const UHmPawnData* InPawnData)
{
	check(InPawnData);
	check(!PawnData);
	PawnData = InPawnData;

	// PawnData의 AbilitySet을 순회하며, ASC에 Ability를 할당(Give)한다
	// 이 과정에서 ASC의 ActivatableAbilities에 추가된다.

	for (UHmAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}
