#pragma once

#include "Components/ControllerComponent.h"
#include "HmCharacterPartTypes.h"
#include "HmControllerComponent_CharacterParts.generated.h"

class UHmPawnComponent_CharacterParts;


USTRUCT()
struct FHmControllerCharacterPartEntry
{
	GENERATED_BODY()

	// CharacterParts에 대한 정의
	UPROPERTY(EditAnywhere)
	FHmCharacterPart Part;

	// CharacterPart 핸들(고유값) = Controller가 Possess하고 있는 Pawn에서 생성한 Character Part의 핸들값
	FHmCharacterPartHandle Handle;
};


// Controller가 Pawn을 Possess 했을 때, Pawn에 어떤 Cosmetic Actor를 생성할 지 결정하는 ControllerComponent

UCLASS(meta = (BlueprintSpawnableComponent))
class UHmControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UHmControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UHmPawnComponent_CharacterParts* GetPawnCustomizer() const;
	void AddCharacterPartInternal(const FHmCharacterPart& NewPart);

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FHmCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	// UFUNCTION으로 정의하지 않으면 AddDynamic이 되지 않는다!!
	// AddDynamic은 결국 UFUNCTION을 바인드한다는 뜻
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FHmControllerCharacterPartEntry> CharacterParts;
};