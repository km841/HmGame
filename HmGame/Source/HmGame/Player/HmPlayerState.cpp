// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPlayerState.h"
#include "HmGame/GameModes/HmExperienceManagerComponent.h"
#include "HmGame/GameModes/HmGameModeBase.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"

PRAGMA_DISABLE_OPTIMIZATION
AHmPlayerState::AHmPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UHmAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}
PRAGMA_ENABLE_OPTIMIZATION

void AHmPlayerState::PostInitializerComponents()
{
	Super::PostInitializeComponents();
	check(AbilitySystemComponent);
	{
		// 처음 InitAbilityActorInfo 호출 당시, OwnerActor와 AvatarActor가 같은 Actor를 가리키고 있으며, 이것은 PlayerState이다.
		// OwnerActor는 PlayerState가 의도한게 맞지만, AvatarActor는 PlayerController가 소유하는 대상인 Pawn이 되어야 한다!
		// 이를 위해 재 세팅을 해준다.

		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	// Avatar Actor를 Nullptr로 밀어버린다!
	// 이 함수를 지금은 Nullptr로 밀어뒀지만, 다시 한번 호출해서 제대로 세팅해주어야 한다!
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
}
