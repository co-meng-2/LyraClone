// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Cm_ExperienceDefinition.generated.h"

class UCm_PawnData;
/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_ExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experience, meta = (AllowedTypes = "Cm_PawnData"))
	TObjectPtr<UCm_PawnData> DefaultPawnData;
};
