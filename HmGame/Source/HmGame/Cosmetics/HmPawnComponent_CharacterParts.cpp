#include "HmPawnComponent_CharacterParts.h"

UHmPawnComponent_CharacterParts::UHmPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FHmCharacterPartHandle UHmPawnComponent_CharacterParts::AddCharacterPart(const FHmCharacterPart& NewPart)
{

	return CharacterPartList.AddEntry(NewPart);
}

FHmCharacterPartHandle FHmCharacterPartList::AddEntry(FHmCharacterPart NewPart)
{
	FHmCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FHmAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;

		if (SpawnActorForEntry(NewEntry))
		{
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}
