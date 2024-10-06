// Fill out your copyright notice in the Description page of Project Settings.


#include "HmHeroComponent.h"

UHmHeroComponent::UHmHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}
