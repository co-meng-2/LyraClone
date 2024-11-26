#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/Cm_GFA_WorldActionBase.h"
#include "Input/Cm_MappableConfigPair.h"
#include "Cm_GFA_AddInputConfig.generated.h"

struct FComponentRequestHandle;
/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_GFA_AddInputConfig : public UCm_GFA_WorldActionBase
{
	GENERATED_BODY()
public:
	/**
	 * UGameFetaureAction interface
	 */
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	/**
	 * UGameFeatureAction_WorldActionBase interface
	 */
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	/** GameFeatureState가 변경될 때, 콜백 함수 (혹은 ExtensionEvent를 GameFrameworkComponentManager를 통해 호출 가능) */
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	/** InputConfig를 EnhancedInputLocalPlayerSubsystem에 추가 및 제거 */
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);

	/** ActiveData를 초기화 */
	void Reset(FPerContextData& ActiveData);

	/** world -> (extension request, pawn) */
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	/** PlayerMappableInputConfig를 들고 있는 멤버 변수 */ 
	UPROPERTY(EditAnywhere)
	TArray<FCm_MappableConfigPair> InputConfigs;
};
