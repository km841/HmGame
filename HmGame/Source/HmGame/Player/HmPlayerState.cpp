// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPlayerState.h"
#include "HmGame/GameModes/HmExperienceManagerComponent.h"
#include "HmGame/GameModes/HmGameModeBase.h"

void AHmPlayerState::PostInitializerComponents()
{
	Super::PostInitializeComponents();

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
