// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Cm_UserFacingExperience.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_UserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experience, meta = (AllowedTypes = "Cm_ExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};