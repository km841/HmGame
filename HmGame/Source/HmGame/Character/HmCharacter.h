// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "HmCharacter.generated.h"

class UHmPawnExtensionComponent;
class UHmCameraComponent;

UCLASS()
class HMGAME_API AHmCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHmCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hm|Character")
	TObjectPtr<UHmPawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hm|Character")
	TObjectPtr<UHmCameraComponent> CameraComponent;
};
