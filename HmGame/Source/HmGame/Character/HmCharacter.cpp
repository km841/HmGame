// Fill out your copyright notice in the Description page of Project Settings.


#include "HmCharacter.h"

// Sets default values
AHmCharacter::AHmCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHmCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHmCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHmCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

