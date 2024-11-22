// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Cm_GameStateBase.generated.h"

class UCm_ExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class LYRACLONE_API ACm_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACm_GameStateBase();
	
	
public:
	UPROPERTY()
	TObjectPtr<UCm_ExperienceManagerComponent> ExperienceManagerComponent;
};
