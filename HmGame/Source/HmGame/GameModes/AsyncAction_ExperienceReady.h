#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Delegates/Delegate.h"
#include "AsyncAction_ExperienceReady.generated.h"

// BP를 통해 UFUNCTION으로 바인딩하는 Multicast Delegate : Dynamic!
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class AGameStateBase;
class UHmExperienceDefinition;

UCLASS()
class UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	// UBlueprintAsyncActionBase Interface
	virtual void Activate() override;

	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const UHmExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();

	// BlueprintAssignable은 BP 상에서 할당 가능한 변수로 정의 하는 것!!
	// 준비가 진짜로 다 되었는가를 확신할 수 있게, 그 이후 블루프린트 노드를 실행할 수 있도록 함
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	// UWorld를 단순 로직 상 캐싱하는 용도
	TWeakObjectPtr<UWorld> WorldPtr;

};