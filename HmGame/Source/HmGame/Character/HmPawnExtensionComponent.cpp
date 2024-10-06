// Fill out your copyright notice in the Description page of Project Settings.


#include "HmPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "HmGame/HmGameplayTags.h"
#include "HmGame/HmLogChannels.h"

const FName UHmPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UHmPawnExtensionComponent::UHmPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

PRAGMA_DISABLE_OPTIMIZATION
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

void UHmPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

bool UHmPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesireState) const
{

	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesireState);
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
