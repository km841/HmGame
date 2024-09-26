// Copyright Epic Games, Inc. All Rights Reserved.

#include "HmGame.h"
#include "HmLogChannels.h"
#include "Modules/ModuleManager.h"

class FHmGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


void FHmGameModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	UE_LOG(LogHm, Warning, TEXT("FHmGameModule"));
}

void FHmGameModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FHmGameModule, Hm, "Hm");