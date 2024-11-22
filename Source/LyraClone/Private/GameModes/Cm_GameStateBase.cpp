// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Cm_GameStateBase.h"

#include "GameModes/Cm_ExperienceManagerComponent.h"

ACm_GameStateBase::ACm_GameStateBase()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UCm_ExperienceManagerComponent>(L"ExperienceManager");
}
