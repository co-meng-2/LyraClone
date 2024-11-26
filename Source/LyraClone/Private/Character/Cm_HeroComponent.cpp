#include "Character/Cm_HeroComponent.h"

#include "Cm_GameplayTags.h"
#include "Cm_LogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/Cm_CameraComponent.h"
#include "Character/Cm_PawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Input/Cm_EnhancedInputComponent.h"
#include "Input/Cm_InputConfig.h"
#include "Player/Cm_PlayerState.h"
#include "Input/Cm_MappableConfigPair.h"
#include "PlayerMappableInputConfig.h"
#include "Player/Cm_PlayerController.h"

const FName UCm_HeroComponent::NAME_ActorFeatureName{"Hero"};
const FName UCm_HeroComponent::NAME_BindInputsNow("BindInputsNow");

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
	const FCm_GameplayTags& InitTags{FCm_GameplayTags::Get()};

	APawn* Loc_Pawn = GetPawn<APawn>();
	const bool bIsLocallyControlled{Loc_Pawn->IsLocallyControlled()};
	const bool bHasAuthority{HasAuthority()};
	
	if(CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		if(bIsLocallyControlled || HasAuthority())
		{
			if(UCm_CameraComponent* CameraComponent{UCm_CameraComponent::FindCameraComponent(Loc_Pawn)})
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}

			if(ACm_PlayerController* Loc_PC{GetController<ACm_PlayerController>()})
			{
				if(Loc_Pawn->InputComponent)
				{
					InitializePlayerInput(Loc_Pawn->InputComponent);
				}
			}
		}
	}
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

TSubclassOf<UCm_CameraMode> UCm_HeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UCm_PawnExtensionComponent* PawnExtComp = UCm_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UCm_PawnData* PawnData = PawnExtComp->GetPawnData<UCm_PawnData>())
		{
			return PawnData->DefaultCameraModeClass;
		}
	}

	return nullptr;
}

void UCm_HeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 MappingContext를 비워준다:
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig 존재 유무 판단:
	if (const UCm_PawnExtensionComponent* PawnExtComp = UCm_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UCm_PawnData* PawnData = PawnExtComp->GetPawnData<UCm_PawnData>())
		{
			if (const UCm_InputConfig* InputConfig = PawnData->Inputconfig)
			{
				const FCm_GameplayTags& GameplayTags = FCm_GameplayTags::Get();

				// HeroComponent 가지고 있는 Input Mapping Context를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가한다
				for (const FCm_MappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						// Binding되는 중에 눌리고 있던 Key에 대해, 다시 뗄 때 까지 Trigger하지 않는다.
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가한다:
						// - AddPlayerMappableConfig를 간단히 보는 것을 추천
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);	
					}
				}

				UCm_EnhancedInputComponent* CmEIComp = CastChecked<UCm_EnhancedInputComponent>(PlayerInputComponent);
				{
					CmEIComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					CmEIComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UCm_HeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UCm_HeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		// X에는 Yaw 값이 있음:
		// - Camera에 대해 Yaw 적용
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y에는 Pitch 값!
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}

void UCm_HeroComponent::BindOnActorInitStateChanged(FName FeatureName, FGameplayTag RequiredState, bool bCallIfReached)
{
	IGameFrameworkInitStateInterface::BindOnActorInitStateChanged(FeatureName, RequiredState, bCallIfReached);
}

void UCm_HeroComponent::RegisterInitStateFeature()
{
	IGameFrameworkInitStateInterface::RegisterInitStateFeature();
}