// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "Cm_ExperienceManagerComponent.generated.h"


class UCm_ExperienceDefinition;

enum class ECmExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCmExperienceLoaded, const UCm_ExperienceDefinition*);

/**
 * ExperienceManagerComponent
 * Experience의 로딩 상태를 관리한다.
 */

UCLASS()
class LYRACLONE_API UCm_ExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:

	bool IsExperienceLoaded(){return (LoadState == ECmExperienceLoadState::Loaded) && (CurrentExperience);}
	
	void CallOrRegister_OnExperienceLoaded(FOnCmExperienceLoaded::FDelegate&& InDelegate);
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();
	const UCm_ExperienceDefinition* GetCurrentExperienceChecked();

public:
	UPROPERTY()
	TObjectPtr<const UCm_ExperienceDefinition> CurrentExperience;
	ECmExperienceLoadState LoadState{ECmExperienceLoadState::Unloaded};

	FOnCmExperienceLoaded OnExperienceLoaded;
};
