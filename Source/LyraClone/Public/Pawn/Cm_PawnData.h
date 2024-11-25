// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Cm_PawnData.generated.h"

class UCm_InputConfig;
class UCm_CameraMode;
/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_PawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cm|Pawn")
	TSubclassOf<APawn> DefaultPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cm|Camera")
	TSubclassOf<UCm_CameraMode> DefaultCameraModeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cm|Input")
	TObjectPtr<UCm_InputConfig> Inputconfig;
};
