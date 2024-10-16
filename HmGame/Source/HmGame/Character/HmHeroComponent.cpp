// Fill out your copyright notice in the Description page of Project Settings.


#include "HmHeroComponent.h"
#include "HmGame/HmLogChannels.h"
#include "HmGame/HmGameplayTags.h"
#include "HmPawnExtensionComponent.h"
#include "HmGame/Player/HmPlayerState.h"
#include "HmGame/Character/HmPawnData.h"
#include "HmGame/Camera/HmCameraComponent.h"
#include "Components/GameFrameworkComponentManager.h"
//#include "HmGame/Input/HmMappingConfigPair.h"

const FName UHmHeroComponent::NAME_ActorFeatureName("Hero");

UHmHeroComponent::UHmHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UHmHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogHm, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
		return;
	}

	RegisterInitStateFeature();
}

void UHmHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Extension Component의 부품이므로, 다른 Component의 상태를 알 필요가 없다! 그의 명령만 수신하도록 함
	BindOnActorInitStateChanged(UHmPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	ensure(TryToChangeInitState(FHmGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UHmHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UHmHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FHmGameplayTags& InitTags = FHmGameplayTags::Get();

	if (Params.FeatureName == UHmPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UHmHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FHmGameplayTags& InitTags = FHmGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	AHmPlayerState* HmPS = GetPlayerState<AHmPlayerState>();

	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
			return true;
	}

	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!HmPS)
		{
			return false;
		}

		return true;
	}

	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		return HmPS && Manager->HasFeatureReachedInitState(Pawn, UHmPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	return false;
}

void UHmHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FHmGameplayTags& InitTags = FHmGameplayTags::Get();

	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AHmPlayerState* HmPS = GetPlayerState<AHmPlayerState>();

		if (!ensure(Pawn && HmPS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UHmPawnData* PawnData = nullptr;

		if (UHmPawnExtensionComponent* PawnExtComp = UHmPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UHmPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			if (UHmCameraComponent* CameraComponent = UHmCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}



	//IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void UHmHeroComponent::CheckDefaultInitialization()
{
	const FHmGameplayTags& InitTags = FHmGameplayTags::Get();

	static const TArray<FGameplayTag> StateChain = {
	InitTags.InitState_Spawned,
	InitTags.InitState_DataAvailable,
	InitTags.InitState_DataInitialized,
	InitTags.InitState_GameplayReady,
	};

	// 내부에서 While문으로 각 상태에 대해 CanChangeInitState를 계~속 호출한다
	ContinueInitStateChain(StateChain);
}

TSubclassOf<UHmCameraMode> UHmHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UHmPawnExtensionComponent* PawnExtComp = UHmPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UHmPawnData* PawnData = PawnExtComp->GetPawnData<UHmPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
