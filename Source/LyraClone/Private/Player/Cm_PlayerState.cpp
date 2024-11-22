// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Cm_PlayerState.h"

#include "GameModes/Cm_ExperienceManagerComponent.h"
#include "GameModes/Cm_GameModeBase.h"


void ACm_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* Loc_GameState{GetWorld()->GetGameState()};
	check(Loc_GameState);

	UCm_ExperienceManagerComponent* ExperienceManager{Loc_GameState->GetComponentByClass<UCm_ExperienceManagerComponent>()};
	check(ExperienceManager);

	ExperienceManager->CallOrRegister_OnExperienceLoaded(FOnCmExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ACm_PlayerState::OnExperienceLoaded(const UCm_ExperienceDefinition* CurrentExperience)
{
	if(ACm_GameModeBase* GameMode{GetWorld()->GetAuthGameMode<ACm_GameModeBase>()})
	{
		const UCm_PawnData* NewPawnData{GameMode->GetPawnDataForController(GetOwningController())};
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}


void ACm_PlayerState::SetPawnData(const UCm_PawnData* InPawnData)
{
	check(InPawnData);
	check(!PawnData);
	
	PawnData = InPawnData;
}

