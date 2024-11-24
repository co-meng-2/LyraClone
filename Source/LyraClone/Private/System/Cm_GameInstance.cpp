// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Cm_GameInstance.h"

#include "Cm_GameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Cm_GameplayTags.h"

void UCm_GameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager{GetSubsystem<UGameFrameworkComponentManager>(this)};
	if (ensure(ComponentManager))
	{
		const FCm_GameplayTags& GameplayTags = FCm_GameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void UCm_GameInstance::Shutdown()
{
	Super::Shutdown();
}
