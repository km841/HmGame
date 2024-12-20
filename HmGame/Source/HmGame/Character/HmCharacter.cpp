// Fill out your copyright notice in the Description page of Project Settings.


#include "HmCharacter.h"
#include "HmPawnExtensionComponent.h"
#include "HmGame/Camera/HmCameraComponent.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"

// Sets default values
AHmCharacter::AHmCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	PawnExtComponent = CreateDefaultSubobject<UHmPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	{
		CameraComponent = CreateDefaultSubobject<UHmCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
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

	// 이 시점에선 반드시 PlayerController가 존재한다!
	PawnExtComponent->SetupPlayerInputComponent();
}

UAbilitySystemComponent* AHmCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetHmAbilitySystemComponent();
}

