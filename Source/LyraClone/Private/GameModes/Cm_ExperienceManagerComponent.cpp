// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Cm_ExperienceManagerComponent.h"

#include "GameModes/Cm_ExperienceDefinition.h"
#include "System/Cm_AssetManager.h"
#include "GameFeaturesSubsystemSettings.h"

void UCm_ExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnCmExperienceLoaded::FDelegate&& InDelegate)
{
	if(IsExperienceLoaded())
	{
		InDelegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(InDelegate));
	}	
}



void UCm_ExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UCm_AssetManager& AssetManager{UCm_AssetManager::Get()};

	TSubclassOf<UCm_ExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		// 왜 UClass로 반환하냐 -> Experience는 BP임. 즉 Class 정보임.
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	const UCm_ExperienceDefinition* Experience = GetDefault<UCm_ExperienceDefinition>(AssetClass);
	check(Experience);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void UCm_ExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ECmExperienceLoadState::Unloaded);

	LoadState = ECmExperienceLoadState::Loading;
	UCm_AssetManager& AssetManager{UCm_AssetManager::Get()};

	// Bundle을 쓰는 이유 -> TryLoad로 이미 ExperienceDef를 load했다.
	// 하지만 이는 관련된 자산까지 포함하지 않는다.
	// 밑은 관련된 자산까지 모두 Memory에 Laod하는 부분이다. 일단 이 정도로 알고 있자. 
	
	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// 로딩할 Asset의 카테고리
	TArray<FName> BundlesToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_Standalone이면? Client/Server 둘다 로딩에 추가된다!
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
	
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

}

void UCm_ExperienceManagerComponent::OnExperienceLoadComplete()
{
	OnExperienceFullLoadCompleted();
}

void UCm_ExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ECmExperienceLoadState::Loaded);

	LoadState = ECmExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UCm_ExperienceDefinition* UCm_ExperienceManagerComponent::GetCurrentExperienceChecked()
{
	check(LoadState == ECmExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
