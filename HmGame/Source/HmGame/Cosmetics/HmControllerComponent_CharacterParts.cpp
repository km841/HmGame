#include "HmControllerComponent_CharacterParts.h"
#include "HmPawnComponent_CharacterParts.h"

UHmControllerComponent_CharacterParts::UHmControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

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
}
