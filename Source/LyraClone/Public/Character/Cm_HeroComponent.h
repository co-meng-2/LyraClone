// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Input/Cm_MappableConfigPair.h"
#include "Cm_HeroComponent.generated.h"

struct FInputActionValue;
class UCm_CameraMode;
/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class LYRACLONE_API UCm_HeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UCm_HeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;
	static const FName NAME_BindInputsNow;

	// ~Begin GameFrameWorkComponent Interface
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~End GameFrameWorkComponent Interface


	// ~Begin IGameFrameworkInitStateInterface
	virtual void RegisterInitStateFeature() override;
	virtual FName GetFeatureName() const override;
	virtual void BindOnActorInitStateChanged(FName FeatureName, FGameplayTag RequiredState, bool bCallIfReached) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	virtual void CheckDefaultInitializationForImplementers() override;
	virtual FGameplayTag ContinueInitStateChain(const TArray<FGameplayTag>& InitStateChain) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void UnregisterInitStateFeature() override;
	// ~End IGameFrameworkInitStateInterface

	TSubclassOf<UCm_CameraMode> DetermineCameraMode() const;
	
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InaputActionValue);
	
	UPROPERTY(EditAnywhere)
	TArray<FCm_MappableConfigPair> DefaultInputConfigs;
};
