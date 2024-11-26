// Fill out your copyright notice in the Description page of Project Settings.


#include "CmCU_SessionSubsystem.h"

#include "Engine/AssetManager.h"

FString UCmCU_HostSessionRequest::GetMapName() const
{
	// AssetManager를 통해, MapID를 통해, FAssetData를 반환하고, 이를 통해 PackageName를 통해 Map 경로를 반환한다
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapId, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}
	return FString();
}

FString UCmCU_HostSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if (ExtraArg.Key.IsEmpty())
		{
			continue;
		}

		// Travel할 Map에 대한 MetaData를 추가한다고 생각하면 된다.
		// 이는 나중에 Parsing하여 사용할 수 있다.
		// ?Experience=DefaultExperience?Option=...
		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}
	
	// Map 경로 앞에 추가하여, 최종 TravelURL 생성
	return FString::Printf(TEXT("%s%s"), *GetMapName(), *CombinedExtraArgs);
}

void UCmCU_SessionSubsystem::HostSession(APlayerController* HostingPlayer, UCmCU_HostSessionRequest* Request)
{
	ULocalPlayer* LocalPlayer = (HostingPlayer != nullptr) ? HostingPlayer->GetLocalPlayer() : nullptr;
	if (!LocalPlayer)
	{
		return;
	}
	
	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}