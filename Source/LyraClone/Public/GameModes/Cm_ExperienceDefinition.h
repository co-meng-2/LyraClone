// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Cm_ExperienceDefinition.generated.h"

class UGameFeatureAction;
class UCm_ExperienceActionSet;
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

	UPROPERTY(EditDefaultsOnly, Category="Cm")
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category="Cm")
	TArray<TObjectPtr<UCm_ExperienceActionSet>> ActionSets;

	UPROPERTY(EditDefaultsOnly, Category="Cm")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
