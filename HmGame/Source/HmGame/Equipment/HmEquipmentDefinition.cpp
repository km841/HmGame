#include "HmEquipmentDefinition.h"
#include "HmEquipmentInstance.h"

UHmEquipmentDefinition::UHmEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 기본 값으로 HmEquipmentInstance를 설정
	InstanceType = UHmEquipmentInstance::StaticClass();
}
