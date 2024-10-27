// Fill out your copyright notice in the Description page of Project Settings.


#include "HmUserFacingExperience.h"
#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* UHmUserFacingExperience::CreateHostingRequest() const
{
    const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

    // UCommonSession_HostSessionRequest는 알아서 GC가 된다.
    // 해당 객체는 해당 프레임에서 사용되기 때문에 상관없지만 다음 프레임에서 사용하게 된다면 Lifetime 관리가 필요하다.
    // 신경써주지 않으면 Dangling 된다!
    UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
    Result->MapID = MapID;
    Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
    
    return Result;
}
