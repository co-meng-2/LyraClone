// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Cm_CameraComponent.generated.h"

class UCm_CameraMode;
class UCm_CameraModeStack;
/**
 * 
 */

DECLARE_DELEGATE_RetVal(TSubclassOf<UCm_CameraMode>, FCm_CameraModeDelegate)

UCLASS()
class LYRACLONE_API UCm_CameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UCm_CameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ~Begin Camera Component interface
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;
	// ~End Camera Component interface

	static UCm_CameraComponent* FindCameraComponent(const APawn* InPawn)
	{
		return InPawn ? InPawn->FindComponentByClass<UCm_CameraComponent>() : nullptr;
	};
	
	void UpdateCameraModes();

	AActor* GetTargetActor() const {return GetOwner();}
	
	UPROPERTY()
	TObjectPtr<UCm_CameraModeStack> CameraModeStack;

	FCm_CameraModeDelegate DetermineCameraModeDelegate; 
};
