// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Cm_GameModeBase.generated.h"

class UCm_PawnData;
class UCm_ExperienceDefinition;
/**
 * 
 */
UCLASS()
class LYRACLONE_API ACm_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACm_GameModeBase();

	// InitGame에서는 GameInstance, World, Level, GameMode까지만 유효한 상태이다.
	// 현재 프레임에서는 Lyra Concept의 Experience를 처리할 수 없다. -> ExperienceManager가 GameState에 있으므로.
	// 따라서 다음 틱에서 실행할 수 있도록 한 틱 뒤로 밀어준다.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);

	virtual void InitGameState() final;
	void OnExperienceLoaded(const UCm_ExperienceDefinition* CurrentExperience);

	// ~ Begin GameModeBase Spawn Player Pawn Interface
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	// ~ End GameModeBase Spawn Player Pawn Interface
	

	bool IsExperienceLoaded();
	// PawnData를 가져오는 역할은 GameModeBase가 담당하도록 정한다.
	// 이를 통해 통일성을 갖출 수 있음.
	const UCm_PawnData* GetPawnDataForController(const AController* InController);
};
