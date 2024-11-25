// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cm_MappableConfigPair.generated.h"
/**
 * 
 */

class UPlayerMappableInputConfig;

USTRUCT()
struct FCm_MappableConfigPair
{
	GENERATED_BODY()

	FCm_MappableConfigPair() = default;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically;
};
