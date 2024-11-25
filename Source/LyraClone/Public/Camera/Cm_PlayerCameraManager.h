// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Cm_PlayerCameraManager.generated.h"



// 여러 곳에서 쓰일 초기화 값
#define CM_CAMERA_DEFAULT_FOV			(80.f)
#define CM_CAMERA_DEFAULT_PITCH_MIN		(-89.f)
#define CM_CAMERA_DEFAULT_PITCH_MAX		(89.f)

/**
 * 
 */
UCLASS()
class LYRACLONE_API ACm_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ACm_PlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
