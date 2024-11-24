// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "ModularGameplay/Public/Components/GameFrameworkInitStateInterface.h"
#include "Pawn/Cm_PawnData.h"
#include "Cm_PawnExtensionComponent.generated.h"

class UCm_PawnData;
/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_PawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UCm_PawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;
	
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

	void SetupPlayerInputComponent();

	// Utility
	template<typename T>
	const T* GetPawnData() const {return Cast<T>(PawnData);}
	void SetPawnData(const UCm_PawnData* InPawnData);
	static UCm_PawnExtensionComponent* FindPawnExtensionComponent(const AActor* InActor)
	{
		return InActor ? InActor->FindComponentByClass<ThisClass>() : nullptr;
	}
	
private:
	TObjectPtr<const UCm_PawnData> PawnData;
};

// /** Information for each registered actor */
// struct FActorFeatureData
// {
// 	/** Actor class for cross referencing with the class callbacks */
// 	TWeakObjectPtr<UClass> ActorClass;
//
// 	/** All active features */
// 	TArray<FActorFeatureState> RegisteredStates;
//
// 	/** All delegates bound to this actor */
// 	TArray<FActorFeatureRegisteredDelegate> RegisteredDelegates;
// };
//
// /** State for a specific object implementing an actor feature, should this be in a map instead of an array? */
// struct FActorFeatureState
// {
// 	FActorFeatureState(FName InFeatureName) : FeatureName(InFeatureName) {}
//
// 	/** The feature this is tracking */
// 	FName FeatureName;
//
// 	/** The state when it was last registered */
// 	FGameplayTag CurrentState;
//
// 	/** The object implementing this feature, this can be null */
// 	TWeakObjectPtr<UObject> Implementer;
// };
//
// struct FActorFeatureRegisteredDelegate
// {
// 	/** Construct from a native or BP Delegate */
// 	FActorFeatureRegisteredDelegate(FActorInitStateChangedDelegate&& InDelegate, FName InFeatureName = NAME_None, FGameplayTag InInitState = FGameplayTag());
// 	FActorFeatureRegisteredDelegate(FActorInitStateChangedBPDelegate&& InDelegate, FName InFeatureName = NAME_None, FGameplayTag InInitState = FGameplayTag());
//
// 	/** Call the appropriate native/bp delegate, this could invalidate this struct */
// 	void Execute(AActor* OwningActor, FName FeatureName, UObject* Implementer, FGameplayTag FeatureState);
//
// 	/** Delegate that is called on notification */
// 	FActorInitStateChangedDelegate Delegate;
//
// 	/** BP delegate that is called on notification */
// 	FActorInitStateChangedBPDelegate BPDelegate;
//
// 	/** A handle assigned to this delegate so it acts like a multicast delegate for removal */
// 	FDelegateHandle DelegateHandle;
//
// 	/** If this is not null, will only activate for specific feature names */
// 	FName RequiredFeatureName;
//
// 	/** If this is not null, will only activate for states >= to this */
// 	FGameplayTag RequiredInitState;
// };