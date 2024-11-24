// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Cm_AssetManager.h"

#include "Cm_GameplayTags.h"
#include "Cm_LogChannels.h"

UCm_AssetManager::UCm_AssetManager(const FObjectInitializer& Initializer)
{
}

UCm_AssetManager& UCm_AssetManager::Get()
{

	check(GEngine);

	if(UCm_AssetManager* SingleTon{Cast<UCm_AssetManager>(GEngine->AssetManager)})
	{
		return *SingleTon;
	}
	
	
	UE_LOG(LogCm, Fatal, L"Invalid AssetManagerClassName in DefaultEngine.ini -> it must be Cm_AssetManager")
	// Dummy
	return *NewObject<UCm_AssetManager>();
}

bool UCm_AssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent{FCommandLine::Get()};
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, L"LogAssetLoads");
	return bLogAssetLoads;
}

void UCm_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCm_GameplayTags::InitializeNativeTags();
}

UObject* UCm_AssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	TUniquePtr<FScopeLogTime> LogTimePtr;
	if(AssetPath.IsValid())
	{
		if(ShouldLogAssetLoads())
		{
			// 인자 : 이름, 누적 시간, 표시 시간 단위
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(L"Synchronous loaded assets [%s]",
				*AssetPath.ToString()), nullptr, FConditionalScopeLogTime::ScopeLog_Milliseconds);
		}
		
		// if(UAssetManager::IsValid()) Deprecated
		if(UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// 매우 느리다. 
		return AssetPath.TryLoad();
	}
	
	return nullptr;
}

void UCm_AssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
