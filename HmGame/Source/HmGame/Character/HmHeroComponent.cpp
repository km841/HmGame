// Fill out your copyright notice in the Description page of Project Settings.


#include "HmHeroComponent.h"
#include "HmGame/HmLogChannels.h"
#include "HmGame/HmGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "HmPawnExtensionComponent.h"
#include "HmGame/Player/HmPlayerState.h"
#include "HmGame/Character/HmPawnData.h"
#include "HmGame/Camera/HmCameraComponent.h"
#include "HmGame/Input/HmMappingConfigPair.h"
#include "HmGame/Input/HmInputComponent.h"
#include "HmGame/Player/HmPlayerController.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"
#include "InputActionValue.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UHmHeroComponent::NAME_ActorFeatureName("Hero");
const FName UHmHeroComponent::NAME_BindInputsNow("BindInputsNow");

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

			PawnExtComp->InitializeAbilitySystem(HmPS->GetHmAbilitySystemComponent(), HmPS);
		}

		if (bIsLocallyControlled && PawnData)
		{
			if (UHmCameraComponent* CameraComponent = UHmCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		if (AHmPlayerController* PC = GetController<AHmPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
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

void UHmHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	// 이걸 가져온 이유는 PlayerController안에 LocalPlayer가 있고, 
	// LocalPlayer 안에 EnhancedInputSubsystem이 있기 때문
	// 그리고 이 EnhancedInputSubsystem 안에 우리의 Input을 등록시켜야 함
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UHmPawnExtensionComponent* PawnExtComp = UHmPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UHmPawnData* PawnData = PawnExtComp->GetPawnData<UHmPawnData>())
		{
			if (const UHmInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FHmGameplayTags& GameplayTags = FHmGameplayTags::Get();

				for (const FHmMappingConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}

				UHmInputComponent* HmIC = CastChecked<UHmInputComponent>(PlayerInputComponent);
				{
					{
						TArray<uint32> BindHandles;
						HmIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
					}

					HmIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					HmIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}

	// GameFeatureAction_AddInputConfig의 HandlePawnExtension에 콜백 함수 전달
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UHmHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		// 현재 핸들링되어 있는 값.
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			// 현재 핸들링된 값(로컬)을 월드로 변환
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			// 이 월드좌표를 기준으로 X 변위량만큼 이동
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			// 현재 핸들링된 값(로컬)을 월드로 변환
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);

			// 이 월드좌표를 기준으로 X 변위량만큼 이동
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}


	}
}

void UHmHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		// X에는 Yaw값이 있다.
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}


}

void UHmHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UHmPawnExtensionComponent* PawnExtComp = UHmPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UHmAbilitySystemComponent* HmASC = PawnExtComp->GetHmAbilitySystemComponent())
			{
				HmASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void UHmHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UHmPawnExtensionComponent* PawnExtComp = UHmPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UHmAbilitySystemComponent* HmASC = PawnExtComp->GetHmAbilitySystemComponent())
			{
				HmASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}
