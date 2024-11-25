// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Cm_PlayerController.h"

#include "Camera/Cm_PlayerCameraManager.h"

ACm_PlayerController::ACm_PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ACm_PlayerCameraManager::StaticClass();
}
