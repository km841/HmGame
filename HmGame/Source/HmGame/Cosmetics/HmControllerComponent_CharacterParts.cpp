#include "HmControllerComponent_CharacterParts.h"
#include "HmPawnComponent_CharacterParts.h"

UHmControllerComponent_CharacterParts::UHmControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UHmControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// Possess되는 Pawn이 변경될 때 불리는 Delegate 바인드
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UHmControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

UHmPawnComponent_CharacterParts* UHmControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 생각해보면 앞서 HmPawnComponent_CharacterParts를 상속 받는 B_MannyquinPawnCosmetics를 이미 B_Hero_ShooterMannyquin에 추가하였다.
		// B_MannyquinCosmetics가 반환되길 기대함
		return ControlledPawn->FindComponentByClass<UHmPawnComponent_CharacterParts>();
	}
	return nullptr;
}

void UHmControllerComponent_CharacterParts::AddCharacterPartInternal(const FHmCharacterPart& NewPart)
{
	FHmControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (UHmPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void UHmControllerComponent_CharacterParts::AddCharacterPart(const FHmCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UHmControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UHmPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FHmControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UHmControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (UHmPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UHmPawnComponent_CharacterParts>() : nullptr)
	{
		for (FHmControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	if (UHmPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UHmPawnComponent_CharacterParts>() : nullptr)
	{
		for (FHmControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}

}
