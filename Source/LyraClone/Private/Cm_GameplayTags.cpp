#include "Cm_GameplayTags.h"

#include "GameplayTagsManager.h"

FCm_GameplayTags FCm_GameplayTags::GameplayTags;

void FCm_GameplayTags::InitializeNativeTags()
{
	GameplayTags.AddAllTags();
}

void FCm_GameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(L"(Native) ") + FString(TagComment));
}

void FCm_GameplayTags::AddAllTags()
{
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");
}
