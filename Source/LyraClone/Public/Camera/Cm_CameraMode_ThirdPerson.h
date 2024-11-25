// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/Cm_CameraMode.h"
#include "Cm_CameraMode_ThirdPerson.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class LYRACLONE_API UCm_CameraMode_ThirdPerson : public UCm_CameraMode
{
	GENERATED_BODY()

public:
	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Cm")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
