// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_WorldActionB.h"

void UGameFeatureAction_WorldActionB::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			AddToWorld(WorldContext, Context);
		}
	}
}