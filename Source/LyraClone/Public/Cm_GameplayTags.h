#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FCm_GameplayTags
{
	static const FCm_GameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags();

	// InitStateTags
	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

	// EnhancedInput Tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	
private:
	static FCm_GameplayTags GameplayTags;
};
