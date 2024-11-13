#include "AsyncAction_ExperienceReady.h"
#include "HmExperienceManagerComponent.h"
#include "HmExperienceDefinition.h"

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* Action = nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Action = NewObject<UAsyncAction_ExperienceReady>();
		Action->WorldPtr = World;
		Action->RegisterWithGameInstance(World);
	}
	return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
	if (UWorld* World = WorldPtr.Get())
	{
		// GameState가 준비되어 있으면 Step1 무시하고 Step2 바로 시작
		if (AGameStateBase* GameState = World->GetGameState())
		{
			Step2_ListenToExperienceLoading(GameState);
		}

		// 아직 준비되어 있지 않으면, UWorld::GameStateSetEvent에 Step1_HandleGameStateSet을 Delegate로 바인딩
		else
		{
			World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
		}
	}

	else
	{
		// 현재 AsnycAction 대상이었던 World가 유효(Valid)하지 않으므로 제거 진행
		// - 코드를 내려가보면, GameInstance에서 레퍼런스 대상으로 제거시킴
		// - UAsyncAction의 캐싱 위치가 GameInstance임을 알 수 있다.
		SetReadyToDestroy();
	}
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* World = WorldPtr.Get())
	{
		World->GameStateSetEvent.RemoveAll(this);
	}

	Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	check(GameState);

	// Experience Loading 상태 정보가 GameState에 붙어 있는 ExperienceManagerComponent에서 가져올 수 있다.
	// 그 때문에 GameStateSetEvent에 바인딩 한 것 
	UHmExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UHmExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// 만약 Experience가 준비되었다면 Step3 스킵!
	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		UWorld* World = GameState->GetWorld();
		check(World);

		// 이미 Experience가 준비되었더라도 미완료된 작업이 있을 수 있으니 다음 틱에 바인딩
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::Step4_BroadcastReady));
	}
	else
	{
		// 준비되지 않았다면 OnExprienceLoaded에 바인딩시킨다.
		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnHmExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::Step3_HandleExperienceLoaded));
	}
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const UHmExperienceDefinition* CurrentExperience)
{
	Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	OnReady.Broadcast();
	SetReadyToDestroy();
}
