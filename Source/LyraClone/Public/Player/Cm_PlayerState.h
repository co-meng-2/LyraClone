// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Cm_PlayerState.generated.h"

class UCm_ExperienceDefinition;
class UCm_PawnData;
/**
 * 
 */
UCLASS()
class LYRACLONE_API ACm_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() final;
	void OnExperienceLoaded(const UCm_ExperienceDefinition* CurrentExperience);
	void SetPawnData(const UCm_PawnData* InPawnData);

	
	template<typename T>
	const T* GetPawnData() const {return Cast<T>(PawnData);}

	// 캐시 용도
	UPROPERTY()
	TObjectPtr<const UCm_PawnData> PawnData;
};
