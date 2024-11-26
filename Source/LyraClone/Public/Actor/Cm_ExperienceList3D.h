#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cm_ExperienceList3D.generated.h"

class ACm_TeleportUserFacingExperience;
struct FStreamableHandle;
class UCm_UserFacingExperience;

UCLASS()
class LYRACLONE_API ACm_ExperienceList3D : public AActor
{
	GENERATED_BODY()
	
public:	
	ACm_ExperienceList3D();

	UFUNCTION()
	void OnPrimaryAssetListLoaded();
protected:
	virtual void BeginPlay() override;
	
private:
	TArray<TObjectPtr<UCm_UserFacingExperience>> Arr_UserFacingExperiences;
	
	UPROPERTY(EditDefaultsOnly, Category="Cm")
	float Offset{20.f};
	
	UPROPERTY(EditDefaultsOnly, Category="Cm")
	TSubclassOf<ACm_TeleportUserFacingExperience> TeleporterClass;
};
