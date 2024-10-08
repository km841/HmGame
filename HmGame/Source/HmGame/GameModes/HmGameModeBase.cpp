// Fill out your copyright notice in the Description page of Project Settings.


#include "HmGameModeBase.h"
#include "HmGameState.h"
#include "HmGame/HmLogChannels.h"
#include "HmGame/Player/HmPlayerController.h"
#include "HmGame/Player/HmPlayerState.h"
#include "HmGame/Character/HmCharacter.h"
#include "HmGame/Character/HmPawnData.h"
#include "HmGame/GameModes/HmExperienceManagerComponent.h"
#include "HmGame/GameModes/HmExperienceDefinition.h"
#include "HmGame/Character/HmPawnExtensionComponent.h"
#include "HmGame/System/HmAssetManager.h"


AHmGameModeBase::AHmGameModeBase()
{
	GameStateClass = AHmGameState::StaticClass();
	PlayerControllerClass  = AHmPlayerController::StaticClass();
	PlayerStateClass = AHmPlayerState::StaticClass();
	DefaultPawnClass = AHmCharacter::StaticClass();
}

void AHmGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


	// 현재 프레임에서는 초기화가 진행되어야 하므로, 다음 프레임에서 이 함수를 수행한다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExceptingOne);
}

void AHmGameModeBase::InitGameState()
{
	Super::InitGameState(); // GameState가 있다는 것을 보장받을 수 있다!

	UHmExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UHmExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnHmExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* AHmGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UHmPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AHmGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* AHmGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UHmPawnExtensionComponent* PawnExtComp = UHmPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UHmPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

void AHmGameModeBase::HandleMatchAssignmentIfNotExceptingOne()
{
	FPrimaryAssetId ExperienceId;

	// precedence order (highest wins)
	// - matchmaking assignment (if present)
	// - default experience

	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("HmExperienceDefinition"), FName("B_HmDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void AHmGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid());

	UHmExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UHmExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

bool AHmGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	UHmExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UHmExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void AHmGameModeBase::OnExperienceLoaded(const UHmExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const UHmPawnData* AHmGameModeBase::GetPawnDataForController(const AController* InController) const
{
	if (InController)
	{
		if (const AHmPlayerState* HmPS = InController->GetPlayerState<AHmPlayerState>())
		{
			if (const UHmPawnData* PawnData = HmPS->GetPawnData<UHmPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	UHmExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UHmExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const UHmExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}


	return nullptr;
}
