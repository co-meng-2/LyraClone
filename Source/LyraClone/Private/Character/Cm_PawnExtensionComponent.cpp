// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Cm_PawnExtensionComponent.h"

#include "Cm_GameplayTags.h"
#include "Cm_LogChannels.h"
#include "Components/GameFrameworkComponentManager.h"

// 이름은 Prefix Component 빼고 만드는듯 함.
const FName UCm_PawnExtensionComponent::NAME_ActorFeatureName{"PawnExtension"};

UCm_PawnExtensionComponent::UCm_PawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCm_PawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	if(!GetPawn<APawn>())
	{
		CM_LOG("This component should be attached to Pawn!");
		return;
	}

	// Owner Pawn이 가진 이 Component에 대한 Map생성.
	RegisterInitStateFeature();

	// FrameworkManager의 Singleton을 얻는 방법.
	// UGameFrameworkComponentManager* FrameworkManager{UGameFrameworkComponentManager::GetForActor(GetOwningActor())};
}

void UCm_PawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Actor에 등록된 FeatureComponent의 InitState 상태를 관찰하겠다!
		// FeatureName과 Tag가 이렇게 기본값으로 정의되어 있으면, 모든 Feature의 모든 상태로의 변경을 감지하겟다는 뜻이다.
		// 이는 PawnExtensionComp가 모든 Component를 관리한다는 설계 원칙과 일치한다.
		// 마지막 인자는 어떤 Feauture의 현재 상태가 지정된 상태와 같으면 바로 Delegate를 실행한다는 의미이다.
		// 여기서 Binding되는 Delegate는 Override해서 지정할 수 있으며, OnActorInitStateChanged이다.
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	ensure(TryToChangeInitState(FCm_GameplayTags::Get().InitState_Spawned));
	
	CheckDefaultInitialization();
}

void UCm_PawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void UCm_PawnExtensionComponent::RegisterInitStateFeature()
{
	IGameFrameworkInitStateInterface::RegisterInitStateFeature();
}


FName UCm_PawnExtensionComponent::GetFeatureName() const
{
	return NAME_ActorFeatureName;
}

void UCm_PawnExtensionComponent::BindOnActorInitStateChanged(FName FeatureName, FGameplayTag RequiredState,
	bool bCallIfReached)
{
	IGameFrameworkInitStateInterface::BindOnActorInitStateChanged(FeatureName, RequiredState, bCallIfReached);
}

UE_DISABLE_OPTIMIZATION
void UCm_PawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// 빈 구현
	// IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
	if(Params.FeatureName != NAME_ActorFeatureName)
	{
		const FCm_GameplayTags& InitTags{FCm_GameplayTags::Get()};
		if(Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}
UE_ENABLE_OPTIMIZATION

void UCm_PawnExtensionComponent::CheckDefaultInitialization()
{
	// 빈 구현
	// IGameFrameworkInitStateInterface::CheckDefaultInitialization();

	// Owner의 모든 Feauture의 Implementer에 대해서 CheckDefaultInitialization을 실행시켜준다.
	// 이때, 이 ExtensionComponent까지 포함하면 무한 재귀가 돌게 되므로, 내부에서 이 Feature에 대한 이름을 가지고 제외시켜주는 부분이 있다.
	CheckDefaultInitializationForImplementers();

	FCm_GameplayTags InitTags{FCm_GameplayTags::Get()};
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	
	ContinueInitStateChain(StateChain);
}

bool UCm_PawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	// 빈 구현
	// return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
	
	check(Manager);

	APawn* Loc_Pawn{GetPawn<APawn>()};
	const FCm_GameplayTags& InitTags{FCm_GameplayTags::Get()};

	if(!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if(Loc_Pawn)
		{
			return true;
		}
	}

	if(CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// GaemMode SpawnAtTransform에서 Deferred Spawn으로 PawnData에 할당해주기 때문에 사실상 막힐 일 없지 않나..
		if(!PawnData)
		{
			return false;
		}

		// 여기서 한 번 막힘 -> PlayerController가 이 Pawn을 Possess하기 전 일 수 있음. (99.9999% 전이 맞다.)
		const bool bHasAuthority{Loc_Pawn->HasAuthority()};
		const bool bIsLocallyControlled{Loc_Pawn->IsLocallyControlled()};
		
		if(bHasAuthority || bIsLocallyControlled)
		{
			if(!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	if(CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(Loc_Pawn, InitTags.InitState_DataAvailable);
	}

	if(CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		CM_LOG("%s : Init Completed", *StaticClass()->GetName())
		return true;
	}

	return false;
}

void UCm_PawnExtensionComponent::CheckDefaultInitializationForImplementers()
{
	IGameFrameworkInitStateInterface::CheckDefaultInitializationForImplementers();
}

// List 처럼 다음 다음 다음으로 진행한다. 중간에 멈추면 다시 실행시켜 주는 부분이 있어야 함.
FGameplayTag UCm_PawnExtensionComponent::ContinueInitStateChain(const TArray<FGameplayTag>& InitStateChain)
{
	return IGameFrameworkInitStateInterface::ContinueInitStateChain(InitStateChain);
}

void UCm_PawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void UCm_PawnExtensionComponent::UnregisterInitStateFeature()
{
	IGameFrameworkInitStateInterface::UnregisterInitStateFeature();
}

void UCm_PawnExtensionComponent::SetupPlayerInputComponent()
{
	// Controller가 Beginplay 한 이후 -> Possess가 완료되었음.
	CheckDefaultInitialization();
}

void UCm_PawnExtensionComponent::SetPawnData(const UCm_PawnData* InPawnData)
{
	APawn* Pawn{GetPawnChecked<APawn>()};
	
	if(Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if(PawnData)
	{
		return;
	}

	PawnData = InPawnData;
}

