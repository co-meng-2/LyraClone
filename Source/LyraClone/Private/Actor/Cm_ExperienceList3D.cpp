#include "Actor/Cm_ExperienceList3D.h"

#include "Actor/Cm_TeleportUserFacingExperience.h"
#include "Engine/AssetManager.h"
#include "GameModes/Cm_UserFacingExperience.h"



ACm_ExperienceList3D::ACm_ExperienceList3D()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACm_ExperienceList3D::OnPrimaryAssetListLoaded()
{
	TArray<UObject*> Arr_Loaded;
	UAssetManager& AssetManager{UAssetManager::Get()};
	AssetManager.GetPrimaryAssetObjectList(L"Cm_UserFacingExperience", Arr_Loaded);
	
	FVector Loc_ActorLoc{GetActorLocation()};
	FVector Loc_ActorLeftDir{-GetActorRightVector()};
	FVector Loc_SpawnStartLoc{Loc_ActorLoc + Loc_ActorLeftDir * (Arr_Loaded.Num() - 1) * Offset / 2.f};
	FActorSpawnParameters Loc_SpawnParams;
	Loc_SpawnParams.Owner = this;
	Loc_SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for(UObject* Elem : Arr_Loaded)
	{
		ACm_TeleportUserFacingExperience* NewActor{
			GetWorld()->SpawnActor<ACm_TeleportUserFacingExperience>(TeleporterClass, Loc_SpawnStartLoc, GetActorRotation(), Loc_SpawnParams)};
		NewActor->SetUserfacingExperience(Cast<UCm_UserFacingExperience>(Elem));
		Loc_SpawnStartLoc += -Loc_ActorLeftDir * Offset;
	}
}

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryAssetListLoaded, const TArray<UObject*>&, Loaded);
void ACm_ExperienceList3D::BeginPlay()
{
	Super::BeginPlay();

	Arr_UserFacingExperiences.Empty();

	TArray<FPrimaryAssetId> Loc_Arr_PrimaryAssetId;
	UAssetManager& AssetManager{UAssetManager::Get()};
	// AssetManager에서 가져오고 싶은 Primary Asset Type에 대해 Id List를 가져온다.
	if(bool Success{AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType(L"Cm_UserFacingExperience"), Loc_Arr_PrimaryAssetId)})
	{
		// AssetManager에 Id List를 줘서 AsyncLoad하고 이에 대해 결과를 받을 Handle을 등록한다.
		AssetManager.LoadPrimaryAssets(Loc_Arr_PrimaryAssetId, TArray<FName>(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnPrimaryAssetListLoaded));
	}
}