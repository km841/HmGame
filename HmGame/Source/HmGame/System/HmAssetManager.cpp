// Fill out your copyright notice in the Description page of Project Settings.


#include "HmAssetManager.h"
#include "HmGame/HmLogChannels.h"

UHmAssetManager::UHmAssetManager()
	: Super()
{
}

UHmAssetManager& UHmAssetManager::Get()
{
	check(GEngine);

	if (UHmAssetManager* Singleton = Cast<UHmAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogHm, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project setting); it must be HmAssetManager"));

	return *NewObject<UHmAssetManager>();
}

void UHmAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

bool UHmAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));

	return bLogAssetLoads;
}

UObject* UHmAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{

	// FSoftObjectPath란 Contents Browser에서 띄워져 있는 에셋들(레벨, 메시 등)의 주소를 들고 있는 클래스
	// 실제 로딩할 때는 이 주소를 참조해서 로딩 진행
	if (AssetPath.IsValid())
	{
		// 현재 스코프의 함수 실행 시간을 체크해주는 함수
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous load assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// UAssetManager = 로딩을 관리하는 하나의 클래스 (동기 및 비동기)
		if (UAssetManager::IsValid())
		{
			// 주소를 통해서 로드 진행
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// 만약에 UAssetManager가 생성되지 않은 상황이라면, 넘어가는 분기
		// 굉장히 느리기 때문에 가급적이면 사용하면 안됨
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UHmAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
