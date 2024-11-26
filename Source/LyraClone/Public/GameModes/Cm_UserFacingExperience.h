#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Cm_UserFacingExperience.generated.h"

class UCmCU_HostSessionRequest;
/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_UserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCmCU_HostSessionRequest* CreateHostRequest() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experience, meta = (AllowedTypes = "Cm_ExperienceDefinition"))
	FPrimaryAssetId ExperienceId;
	
};