// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "Cm_ExperienceManagerComponent.generated.h"


namespace UE::GameFeatures
{
	struct FResult;
}

class UCm_ExperienceDefinition;

enum class ECm_ExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
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

	bool IsExperienceLoaded(){return (LoadState == ECm_ExperienceLoadState::Loaded) && (CurrentExperience);}
	
	void CallOrRegister_OnExperienceLoaded(FOnCmExperienceLoaded::FDelegate&& InDelegate);
	// 사용할 Experience를 Load하여 CDO로 CurrentExperience에 넣어줌.
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	// 
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();
	const UCm_ExperienceDefinition* GetCurrentExperienceChecked();

public:
	UPROPERTY()
	TObjectPtr<const UCm_ExperienceDefinition> CurrentExperience;
	ECm_ExperienceLoadState LoadState{ECm_ExperienceLoadState::Unloaded};

	FOnCmExperienceLoaded OnExperienceLoaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
