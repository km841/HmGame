#include "HmGameplayAbility_RangedWeapon.h"
#include "HmRangedWeaponInstance.h"
#include "HmGame/AbilitySystem/HmAbilitySystemComponent.h"
#include "HmGame/AbilitySystem/HmGameplayAbilityTargetData_SingleTargetHit.h"
#include "HmGame/Physics/HmCollisionChannels.h"

UHmGameplayAbility_RangedWeapon::UHmGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

int32 UHmGameplayAbility_RangedWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults) const
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Idx;
		}
		else
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();

			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				return Idx;
			}
		}

	}
	return INDEX_NONE;
}

void UHmGameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;

		// GetAttachedActors느 Recursively하게 모든 Actor를 추출한다.
		Avatar->GetAttachedActors(AttachedActors);

		// 이 액터들의 콜리전은 무시하도록 다 등록한다.
		TraceParams.AddIgnoredActors(AttachedActors);
	}
}

ECollisionChannel UHmGameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const
{
	return Hm_TraceChannel_Weapon;
}

FHitResult UHmGameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;

	// Complex Geometry로 Trace를 진행하며, AvatarActor를 AttachParent를 가지는 오브젝트와의 충돌은 무시한다.
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;

	// Avatar Actor에 부착된 Actors를 찾아 IgnoredActors에 추가한다.
	AddAdditionalTraceIgnoreActors(TraceParams);

	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);

	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (HitResults.Num() > 0)
	{
		for (FHitResult& CurHitResult : HitResults)
		{
			auto Pred = [&CurHitResult](const FHitResult& Other)
				{
					return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
				};

			if (!OutHitResults.ContainsByPredicate(Pred))
			{
				OutHitResults.Add(CurHitResult);
			}
		}

		Hit = OutHitResults.Last();
	}
	else
	{
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}

	return Hit;
}

FHitResult UHmGameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const
{
	FHitResult Impact;

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		Impact = WeaponTrace(StartTrace, EndTrace, 0.0f, bIsSimulated, OutHits);
	}

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, SweepHits);

			const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);

			if (SweepHits.IsValidIndex(FirstPawnIdx))
			{
				bool bUseSweepHits = true;
				for (int32 Idx = 0; Idx < FirstPawnIdx; ++Idx)
				{
					const FHitResult& CurHitResult = SweepHits[Idx];
					auto Pred = [&CurHitResult](const FHitResult& Other)
						{
							return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
						};

					if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
					{
						bUseSweepHits = false;
						break;
					}
				}

				if (bUseSweepHits)
				{
					OutHits = SweepHits;
				}
			}
		}
	}


	// 이어서 계속

	return Impact;
}

void UHmGameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput & InputData, TArray<FHitResult>&OutHits)
{
	UHmRangedWeaponInstance* WeaponData = InputData.WeaponData;
	check(WeaponData);

	// 유호 사거리를 고려하여 EndTrace 정하기
	const FVector BulletDir = InputData.AimDir;
	const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->MaxDamageRange);

	// HitLocation의 초기화 값으로 EndTrace로 설정
	FVector HitLocation = EndTrace;

	// 총알을 하나 Trace 진행
	// Lyra의 경우 샷건과 같은 Cartridge에 여러 개의 총알이 있을 경우를 처리하기 위해 for-loop를 활용하여 복수개 Bullet을 Trace 한다
	TArray<FHitResult> AllImpacts;

	// 실제 레이를 쏘는 부분
	FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, EndTrace, WeaponData->BulletTraceWeaponRadius, false, AllImpacts);

	const AActor* HitActor = Impact.GetActor();
	if (HitActor)
	{
		if (AllImpacts.Num() > 0)
		{
			OutHits.Append(AllImpacts);
		}

		HitLocation = Impact.ImpactPoint;
	}

	if (OutHits.Num() == 0)
	{
		if (!Impact.bBlockingHit)
		{
			Impact.Location = EndTrace;
			Impact.ImpactPoint = EndTrace;
		}

		OutHits.Add(Impact);
	}
}

UHmRangedWeaponInstance* UHmGameplayAbility_RangedWeapon::GetWeaponInstance()
{
	return Cast<UHmRangedWeaponInstance>(GetAssociatedEquipment());
}

FVector UHmGameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	// 미구현인 듯... Weapon이 아닌 Pawn의 위치를 가져온다
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	const FVector SourceLoc = AvatarPawn->GetActorLocation();
	return SourceLoc;
}

FTransform UHmGameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, EHmAbilityTargetingSource Source)
{
	check(SourcePawn);
	check(Source == EHmAbilityTargetingSource::CameraTowardsFocus);

	// 아래 로직은 CameraTowardsFocus만 추출한 로직

	AController* Controller = SourcePawn->Controller;
	if (Controller == nullptr)
	{
		return FTransform();
	}

	// 매직넘버..
	double FocalDistance = 1024.0f;
	FVector FocalLoc;
	FVector CamLoc;
	FRotator CamRot;

	// PlayerController로부터, Location과 Rotation의 정보를 가져옴
	APlayerController* PC = Cast<APlayerController>(Controller);
	check(PC);
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	FVector AimDir = CamRot.Vector().GetSafeNormal();
	FocalLoc = CamLoc + (AimDir * FocalDistance);

	// WeaponLoc이 아닌 Pawn의 Loc
	const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
	FVector FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);

#if 1
	{
		// WeaponLoc (사실상 ActorLoc)
		DrawDebugPoint(GetWorld(), WeaponLoc, 10.0f, FColor::Red, false, 60.0f);
		// CamLoc
		DrawDebugPoint(GetWorld(), CamLoc, 10.0f, FColor::Yellow, false, 60.0f);
		// FinalCamLoc
		DrawDebugPoint(GetWorld(), FinalCamLoc, 10.0f, FColor::Magenta, false, 60.0f);

		// (WeaponLoc - FocalLoc)
		DrawDebugLine(GetWorld(), FocalLoc, WeaponLoc, FColor::Yellow, false, 60.0f, 0, 2.0f);
		// (AimDir)
		DrawDebugLine(GetWorld(), CamLoc, FocalLoc, FColor::Blue, false, 60.0f, 0, 2.0f);

		// Project Direction Line
		DrawDebugLine(GetWorld(), WeaponLoc, FinalCamLoc, FColor::Red, false, 60.0f, 0, 2.0f);
	}
#endif

	return FTransform(CamRot, FinalCamLoc);
}

void UHmGameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	
	UHmRangedWeaponInstance* WeaponData = GetWeaponInstance();
	if (AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData)
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = WeaponData;
		InputData.bCanPlayBulletFX = true;

		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn, EHmAbilityTargetingSource::CameraTowardsFocus);
		// 언리얼은 Forward Vector가 (1, 0, 0)이다! EAxis::X이다
		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();
		InputData.EndAim = InputData.StartTrace + InputData.AimDir * WeaponData->MaxDamageRange;

#if 0
		{
			static float DebugThickness = 2.0f;
			DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.0f), FColor::Yellow, false, 10.0f, 0, DebugThickness);
		}
#endif

		TraceBulletsInCartridge(InputData, OutHits);
	}
}

void UHmGameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilitySystemComponent);

	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			OnRangeWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			K2_EndAbility();
		}
	}
}

void UHmGameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
	// ActorInfo는 AbilitySet에서 GiveAbility 호출로 설정된다
	// - UGameplayAbility::OnGiveAbility() 에서 SetCurrentActorInfo()에서 설정된다
	// - AbilitySystemComponent::GiveAbility()에서 OnGiveAbility() 호출한다
	// - HmAbilitySet::GiveToAbilitySystem() 에서 GiveAbility()를 호출한다
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();

	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	// 여기서 Lyra의 샷건 처리와 같은 탄착 처리는 생략하고, 권총으로 진행 (간단 버전)

	// 총알 궤적의 Hit 정보 계산
	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);

	// GameplayAbilityTargetData는 Server/Client 간 Ability의 공유 데이터로 이해하면 된다.
	// - 허나, 우리는 싱글 플레이어이므로 Ability의 데이터로 이해하면 됨 (현재는 큰 의미가 없음)
	
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = 0;

	if (FoundHits.Num() > 0)
	{
		// Cartridge는 일반 권총의 경우, 탄약에 하나의 총알이 들어 있지만, 샷건의 경우 여러 개의 총알이 있다, 한마디로 탄약을 카트리지라고 생각하면 됨
		const int32 CartridgeID = FMath::Rand();
		for (const FHitResult& FoundHit : FoundHits)
		{

			// AbilityTargetData에 SingleTargetHit 정보를 담는다
			// 참고로 TargetData.Add의 경우, SharedPtr에 넣기 때문에 new는 신경쓰지 않아도 됨
			FHmGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FHmGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
			TargetData.Add(NewTargetData);
		}
	}

	// 가공된 AbilityTargetData가 준비되었으므로, OnTargetDataReadyCallback을 호출
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}
