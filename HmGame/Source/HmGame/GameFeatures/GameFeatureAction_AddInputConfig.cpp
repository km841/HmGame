// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "Components/GameFrameworkComponentManager.h"
#include "HmGame/Character/HmHeroComponent.h"

void UGameFeatureAction_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty() ||
		!ensure(ActiveData.PawnsAddedTo.IsEmpty())))
	{
		Reset(ActiveData);
	}

	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputConfig::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

void UGameFeatureAction_AddInputConfig::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (GameInstance && World && World->IsGameWorld())
	{
		// GFCM을 이용하여 ExtensionHandler를 추가하여 등록 진행
		// - HandlePawnExtension 콜백 함수로 연결
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);

			// 등록된 콜백 함수의 핸들을 ActiveData의 ExtensionRequestHandles에 등록
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentMan->AddExtensionHandler(APawn::StaticClass(), AddConfigDelegate);
			ActiveData.ExtensionRequestHandles.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddInputConfig::HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	APawn* AsPawn = CastChecked<APawn>(Actor);
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UHmHeroComponent::NAME_BindInputsNow)
	{
		AddInputConfig(AsPawn, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UHmHeroComponent::NAME_BindInputsNow)
	{
		RemoveInputConfig(AsPawn, ActiveData);
	}
}

void UGameFeatureAction_AddInputConfig::AddInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = false;

			for (const FHmMappingConfigPair& Pair : InputConfigs)
			{
				if (Pair.bShouldActivateAutomatically)
				{
					Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
				}
			}

			ActiveData.PawnsAddedTo.AddUnique(Pawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (const FHmMappingConfigPair& Pair : InputConfigs)
			{
				Subsystem->RemovePlayerMappableConfig(Pair.Config.LoadSynchronous());
			}

			ActiveData.PawnsAddedTo.Remove(Pawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::Reset(FPerContextData& ActiveData)
{
	ActiveData.ExtensionRequestHandles.Empty();

	while (!ActiveData.PawnsAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnsAddedTo.Top();
		if (PawnPtr.IsValid())
		{
			RemoveInputConfig(PawnPtr.Get(), ActiveData);
		}
		else
		{
			ActiveData.PawnsAddedTo.Pop();
		}
	}
}
