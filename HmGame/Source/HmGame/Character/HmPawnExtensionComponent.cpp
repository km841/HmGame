// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "HmGame/HmGameplayTags.h"
#include "HmGame/HmLogChannels.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"

const FName UHmPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UHmPawnExtensionComponent::UHmPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

PRAGMA_DISABLE_OPTIMIZATION
void UHmPawnExtensionComponent::SetPawnData(const UHmPawnData* InPawnData)
{
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	PawnData = InPawnData;
}
void UHmPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}
void UHmPawnExtensionComponent::InitializeAbilitySystem(UHmAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC && InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		return;
	}

	if (AbilitySystemComponent)
	{
		UnInitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InASC->GetAvatarActor();
	check(!ExistingAvatar);

	// ASC 업데이트
	// InitAbilityActorInfo를 Pawn과 함께 호출, AvatarActor를 Pawn과 함꼐 업데이트함
	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

}
void UHmPawnExtensionComponent::UnInitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent = nullptr;
}
void UHmPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogHm, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
		return;
	}

	// GameFrameworkComponentManager의 InitState 사용을 위한 등록 진행
	// Owning Object(Character)와 Component들을 Manager에 저장한다.
	// Component들을 구분하는 방법은 FeatureName으로 구분하고, Implementer는 this 포인터, 즉 컴포넌트 자신이 들어간다.
	RegisterInitStateFeature();

	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}
PRAGMA_ENABLE_OPTIMIZATION

void UHmPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// NAME_None은 Owning Actor에 등록된 Feature들의 상태가 변하면 모두 관찰하겠다는 뜻
	// FGameplayTag()를 넣으면 모든 상태 (InitState_Spawned, Init...)를 다 관찰하겠다는 뜻
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	
	// InitState_Spawned로 넘어가려고 시도한다!
	ensure(TryToChangeInitState(FHmGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UHmPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

// 관찰하고 있는 Component의 상태가 변경되면 이 함수가 호출된다.
void UHmPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// 나 자신을 제외
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FHmGameplayTags& InitTags = FHmGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}

	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

bool UHmPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FHmGameplayTags& InitTags = FHmGameplayTags::Get();

	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
			return true;
	}

	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!PawnData)
		{
			return false;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	// PawnExtensionComponent는 한 컴포넌트만 무작정 업데이트가되지 않도록, 배리어 역할을 해준다.
	// 각각의 하위 Component들은 다른 같은 등급의 Component들이 Available이 되었는지 확인할 수 없다.
	// 다만 한 가지 확인 할 수 있는 것은 PawnExtensionComponent가 Initialized 상태가 되었다는 것은 다른 모든 하위 Component들도
	// 모두 Available이 되었다는 이야기이므로, 하위 컴포넌트들은 Extension Component가 Initialized 상태인 것만 확인하면
	// 자신의 상태를 Initialized로 변경한다.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Pawn이 소유한 모든 컴포넌트가 InitState_DataAvailable이라면 true를 반환
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}


	return false;
}

void UHmPawnExtensionComponent::CheckDefaultInitialization()
{
	// 자신을 제외한 연결된 컴포넌트들의 CheckDefaultInitialization을 전부 호출한다
	CheckDefaultInitializationForImplementers();

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
