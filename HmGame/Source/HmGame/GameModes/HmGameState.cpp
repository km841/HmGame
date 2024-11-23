// Fill out your copyright notice in the Description page of Project Settings.


#include "HmGameState.h"
#include "HmExperienceManagerComponent.h"

AHmGameState::AHmGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UHmExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));

}
