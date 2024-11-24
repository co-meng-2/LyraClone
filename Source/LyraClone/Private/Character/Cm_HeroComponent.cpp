#include "Character/Cm_HeroComponent.h"

#include "Cm_GameplayTags.h"
#include "Cm_LogChannels.h"
#include "Character/Cm_PawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/Cm_PlayerState.h"

const FName UCm_HeroComponent::NAME_ActorFeatureName{"Hero"};

UCm_HeroComponent::UCm_HeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCm_HeroComponent::OnRegister()
{
	Super::OnRegister();

	if(!GetPawn<APawn>())
	{
		CM_LOG("This component should attach to Pawn!");
		return;
	}

	// Owner Pawn이 가진 이 Component에 대한 Map생성.
	RegisterInitStateFeature();
}

void UCm_HeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(UCm_PawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	ensure(TryToChangeInitState(FCm_GameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UCm_HeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}


FName UCm_HeroComponent::GetFeatureName() const
{
	return NAME_ActorFeatureName;
}

void UCm_HeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FCm_GameplayTags& InitTags{FCm_GameplayTags::Get()};

	
	if (Params.FeatureName == UCm_PawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

UE_DISABLE_OPTIMIZATION
bool UCm_HeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	const FCm_GameplayTags& InitTags = FCm_GameplayTags::Get();
	APawn* Loc_Pawn = GetPawn<APawn>();
	ACm_PlayerState* Loc_PS = GetPlayerState<ACm_PlayerState>();

	// Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Loc_Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		return !!Loc_PS;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 선행
		return Loc_PS && Manager->HasFeatureReachedInitState(Loc_Pawn, UCm_PawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		CM_LOG("%s : Init Completed", *StaticClass()->GetName())
		return true;
	}

	return false;
}
UE_ENABLE_OPTIMIZATION

void UCm_HeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	// HeroComponent는 Player Side, Input, Camera등을 관리하는 Component이다.
	// 이 부분에서 각 State마다 초기화, 관리할 내용이 추가될 것이다.
}

void UCm_HeroComponent::CheckDefaultInitialization()
{
	const FCm_GameplayTags& InitTags = FCm_GameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

void UCm_HeroComponent::CheckDefaultInitializationForImplementers()
{
	IGameFrameworkInitStateInterface::CheckDefaultInitializationForImplementers();
}

FGameplayTag UCm_HeroComponent::ContinueInitStateChain(const TArray<FGameplayTag>& InitStateChain)
{
	return IGameFrameworkInitStateInterface::ContinueInitStateChain(InitStateChain);
}



void UCm_HeroComponent::UnregisterInitStateFeature()
{
	IGameFrameworkInitStateInterface::UnregisterInitStateFeature();
}

void UCm_HeroComponent::BindOnActorInitStateChanged(FName FeatureName, FGameplayTag RequiredState, bool bCallIfReached)
{
	IGameFrameworkInitStateInterface::BindOnActorInitStateChanged(FeatureName, RequiredState, bCallIfReached);
}

void UCm_HeroComponent::RegisterInitStateFeature()
{
	IGameFrameworkInitStateInterface::RegisterInitStateFeature();
}