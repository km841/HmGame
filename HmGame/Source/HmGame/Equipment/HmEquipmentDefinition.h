#pragma once

#include "Containers/Array.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "HmEquipmentDefinition.generated.h"

class UHmEquipmentInstance;
class UHmAbilitySet;

USTRUCT()
struct FHmEquipmentActorToSpawn
{
	GENERATED_BODY()

	// Spawn할 대상 Actor 클래스 (Asset을 상속 받은 Actor로 생각해도 무방)
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	// 어느 Bone Socket에 붙일 지 결정한다!
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	// Socket에서 어느 정도 Transformation을 더할 것인지 결정 (SRT 행렬)
	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttackTransform;


};

UCLASS(BlueprintType, Blueprintable)
class UHmEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UHmEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	// 해당 메타 데이터를 사용하면 어떤 인스턴스를 Spawn할 지 결정하는 클래스
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UHmEquipmentInstance> InstanceType;

	// 해당 장착 아이템을 사용하면 어떤 Actor가 Spawn되는지에 대한 데이터르 담고 있다!
	// Lyra의 Component 구조는 Actor와 Actor의 연결이므로 어떤 Actor를 소환해야 하는데? 에 대한 내용
	// TArray로 들고 있는 이유는 만약 상의/하의가 한 세트인 장비라고 한다면, 두 개의 메쉬가 쌍을 이루어야 하므로..
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FHmEquipmentActorToSpawn> ActorsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<UHmAbilitySet>> AbilitySetsToGrant;

};