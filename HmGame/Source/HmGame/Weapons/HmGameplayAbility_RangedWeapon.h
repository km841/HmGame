#pragma once

#include "CoreMinimal.h"
#include "HmGame/Equipment/HmGameplayAbility_FromEquipment.h"
#include "HmGameplayAbility_RangedWeapon.generated.h"

class UHmRangedWeaponInstance;

// 어디를 대상으로 Target을 설정할지 옵션
UENUM(BlueprintType)
enum class EHmAbilityTargetingSource : uint8
{
	// Camera 기준으로 진행
	CameraTowardsFocus,
};

UCLASS()
class UHmGameplayAbility_RangedWeapon : public UHmGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		UHmRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{}
	};

	UHmGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults) const;

	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);

	UHmRangedWeaponInstance* GetWeaponInstance();

	FVector GetWeaponTargetingSourceLocation() const;

	FTransform GetTargetingTransform(APawn* SourcePawn, EHmAbilityTargetingSource Source);

	void PerformLocalTargeting(TArray<FHitResult>& OutHits);

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
};