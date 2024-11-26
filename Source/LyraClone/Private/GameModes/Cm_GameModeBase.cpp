// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Cm_GameModeBase.h"

#include "Cm_LogChannels.h"
#include "Character/Cm_Character.h"
#include "Character/Cm_PawnExtensionComponent.h"
#include "GameModes/Cm_ExperienceDefinition.h"
#include "GameModes/Cm_ExperienceManagerComponent.h"
#include "GameModes/Cm_GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Cm_PlayerController.h"
#include "Player/Cm_PlayerState.h"
#include "Pawn/Cm_PawnData.h"

ACm_GameModeBase::ACm_GameModeBase()
{
	GameStateClass = ACm_GameStateBase::StaticClass();
	PlayerControllerClass = ACm_PlayerController::StaticClass();
	PlayerStateClass = ACm_PlayerState::StaticClass();
	DefaultPawnClass = ACm_Character::StaticClass();
	// HUDClass = ACm_HUD::StaticClass();
}

void ACm_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}



void ACm_GameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;

	// Lyra에서는 여러 분기로 이루어짐.

	if(!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, L"Experience"))
	{
		const FString ExperienceFromOptions{UGameplayStatics::ParseOption(OptionsString, L"Experience")};
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UCm_ExperienceDefinition::StaticClass()->GetFName()), FName(ExperienceFromOptions));
	}
		
	
	if(!ExperienceId.IsValid())
	{
		// FPrimaryAssetId는 유형(Type)과 이름(Name)으로 고유하게 Asset을 식별할 수 있다.
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("Cm_ExperienceDefinition"), FName("B_CmDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void ACm_GameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid());

	UCm_ExperienceManagerComponent* ExperienceManager = GameState->FindComponentByClass<UCm_ExperienceManagerComponent>();
	ExperienceManager->ServerSetCurrentExperience(ExperienceId);
}

void ACm_GameModeBase::InitGameState()
{
	Super::InitGameState();

	UCm_ExperienceManagerComponent* ExperienceManager = GameState->FindComponentByClass<UCm_ExperienceManagerComponent>();
	check(ExperienceManager);

	ExperienceManager->CallOrRegister_OnExperienceLoaded(FOnCmExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ACm_GameModeBase::OnExperienceLoaded(const UCm_ExperienceDefinition* CurrantExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PlayerController가 Pawn을 Possess하지 않았다면, RestartPlayer를 통해 Pawn을 다시 Spawn한다
		// - 한번 OnPossess를 보도록 하자:
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}


void ACm_GameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if(IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

UClass* ACm_GameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UCm_PawnData* PawnData{GetPawnDataForController(InController)})
	{
		if (PawnData->DefaultPawnClass)
		{
			return PawnData->DefaultPawnClass;
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ACm_GameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}

APawn* ACm_GameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
                                                                    const FTransform& SpawnTransform)
{
	// UE_LOG(LogCm, Warning, L"%hs() Called", __FUNCTION__);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// FindPawnExtensionComponent 구현
			if (UCm_PawnExtensionComponent* PawnExtComp = UCm_PawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UCm_PawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

bool ACm_GameModeBase::IsExperienceLoaded()
{
	check(GameState);
	UCm_ExperienceManagerComponent* ExperienceManager{ GameState->GetComponentByClass<UCm_ExperienceManagerComponent>()};
	check(ExperienceManager);

	return ExperienceManager->IsExperienceLoaded();
}

const UCm_PawnData* ACm_GameModeBase::GetPawnDataForController(const AController* InController)
{
	if(InController)
	{
		if(const ACm_PlayerState* CmPS{InController->GetPlayerState<ACm_PlayerState>()})
		{
			if(const UCm_PawnData* PawnData{CmPS->GetPawnData<UCm_PawnData>()})
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	UCm_ExperienceManagerComponent* Experiencemanager{GameState->GetComponentByClass<UCm_ExperienceManagerComponent>()};
	if (Experiencemanager->IsExperienceLoaded())
	{
		// GetExperienceChecked 구현
		const UCm_ExperienceDefinition* Experience = Experiencemanager->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스에도 핸들링 안되었으면 nullptr
	return nullptr;
}