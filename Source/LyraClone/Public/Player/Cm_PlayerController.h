// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Cm_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ACm_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACm_PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
