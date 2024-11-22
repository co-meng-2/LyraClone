// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Cm_PawnData.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_PawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cm|Pawn")
	TSubclassOf<APawn> PawnClass;
};
