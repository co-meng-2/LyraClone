// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cm_TeleportUserFacingExperience.generated.h"

class UCm_UserFacingExperience;

UCLASS()
class LYRACLONE_API ACm_TeleportUserFacingExperience : public AActor
{
	GENERATED_BODY()
	
public:	
	ACm_TeleportUserFacingExperience();

	void SetUserfacingExperience(UCm_UserFacingExperience* InUserFacingExperience)
	{
		UserFacingExperience = InUserFacingExperience;
	};

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<const UCm_UserFacingExperience> UserFacingExperience;
};
