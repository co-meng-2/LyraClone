// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CmCU_SessionSubsystem.generated.h"


UCLASS(BlueprintType)
class CM_COMMONUSER_API UCmCU_HostSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	/** MapID -> Text 변환 */
	FString GetMapName() const;

	/** ServerTravel에 전달할 최종 URL을 생성한다 */
	FString ConstructTravelURL() const;

	/** 준비할 MapID (맵 경로) */
	UPROPERTY(BlueprintReadWrite, Category = Session)
	FPrimaryAssetId MapId;

	/** 전달할 CmdArgs (Experience 이름을 전달함) -> 곧 UserFacingExperienceDefinition에서 보게 될 것임 */
	UPROPERTY(BlueprintReadWrite, Category = Session)
	TMap<FString, FString> ExtraArgs;
};


UCLASS()
class CM_COMMONUSER_API UCmCU_SessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UCmCU_SessionSubsystem() {}

	UFUNCTION(BlueprintCallable, Category = Session)
	void HostSession(APlayerController* HostingPlayer, UCmCU_HostSessionRequest* Request);

	/**
	 * member variables
	 */
	/** PendingTravelURL은 흔히 맵의 경로로 생각하면 된다 */
	FString PendingTravelURL;
};
