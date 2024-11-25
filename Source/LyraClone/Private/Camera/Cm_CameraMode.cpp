// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Cm_CameraMode.h"

#include "Camera/Cm_CameraComponent.h"
#include "Camera/Cm_PlayerCameraManager.h"

FCm_CameraModeView::FCm_CameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(CM_CAMERA_DEFAULT_FOV)
{
}

void FCm_CameraModeView::Blend(const FCm_CameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		// Weight가 1.0이면 Other를 덮어쓰면 된다
		*this = Other;
		return;
	}

	// Location + OtherWeight * (Other.Location - Location);
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	// Location과 같은 방식 Lerp (ControlRotation과 FieldOfView도 같음)
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

UCm_CameraMode::UCm_CameraMode(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	FieldOfView = CM_CAMERA_DEFAULT_FOV;
	ViewPitchMin = CM_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = CM_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = ECm_CameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

void UCm_CameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void UCm_CameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch 값에 대해 Min/Max를 Clamp시킴
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FHakCameraModeView에 PivotLocation/Rotation 설정
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 똑같이 ControlRotation으로 활용
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void UCm_CameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f)
	{
		// BlendTime은 Blending 과정 총 시간(초)
		// - BlendAlpha는 0 -> 1로 변화하는 과정:
		// - DeltaTime을 활용하여, BlendTime을 1로 볼 경우, 진행 정도를 누적
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	// BlendFucntion에 따라 x축이 BlendAlpha라면 가중치인 BlendWeight는 y값이다.
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case ECm_CameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ECm_CameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ECm_CameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ECm_CameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

FVector UCm_CameraMode::GetPivotLocation()
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight를 고려하여, ViewLocation을 반환함
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UCm_CameraMode::GetPivotRotation()
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRoation() 확인, 보통 Pawn의 ControlRotation을 반환
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

AActor* UCm_CameraMode::GetTargetActor()
{
	const UCm_CameraComponent* Loc_CamerComponent = GetCmCameraComponent();
	return Loc_CamerComponent->GetTargetActor();
}

const UCm_CameraComponent* UCm_CameraMode::GetCmCameraComponent()
{
	return Cast<UCm_CameraComponent>(GetOuter());
}


UCm_CameraModeStack::UCm_CameraModeStack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCm_CameraModeStack::PushCameraMode(TSubclassOf<UCm_CameraMode>& InCameraModeClass)
{
	if (!InCameraModeClass)
	{
		return;
	}

	// Cache된 거 있으면 그 Mode를 가져오고 아니면 생성해서 반환.
	UCm_CameraMode* CameraMode = GetCameraModeInstance(InCameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	// 
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}

	// 1.0 에 대해 CameraModeStack이 파이를 나눠가진다.
	// 이때, 순서대로 남은 파이의 BlendWeight * 100% 만큼 가져간다.
	int32 ExistingStackIndex = INDEX_NONE;

	// 존재하던 CameraMode의 BlendWeight Cache용
	float ExistingStackContribution = 1.0f;
	
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}

	// 지우고 앞에 넣기 위해.
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;
	
	CameraModeStack.Insert(CameraMode, 0);
	// 마지막은 항상 1.0 이어야 함 -> 파이 나눠먹기!
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

UCm_CameraMode* UCm_CameraModeStack::GetCameraModeInstance(TSubclassOf<UCm_CameraMode>& InCameraModeClass)
{
	check(InCameraModeClass);
	
	for (UCm_CameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == InCameraModeClass))
		{
			return CameraMode;
		}
	}

	UCm_CameraMode* NewCameraMode = NewObject<UCm_CameraMode>(GetOuter(), InCameraModeClass);
	CameraModeInstances.Add(NewCameraMode);
	return NewCameraMode;
}

void UCm_CameraModeStack::EvaluateStack(float DeltaTime, FCm_CameraModeView& OutCameraModeView)
{
	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);
}

void UCm_CameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack을 순회하며, CameraMode를 업데이트한다
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UCm_CameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// UpdateCameraMode를 확인해보자:
		CameraMode->UpdateCameraMode(DeltaTime);

		// 만약 하나라도 CameraMode가 BlendWeight가 1.0에 도달했다면, 그 이후 CameraMode를 제거한다
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		// 생각해보면 이거 때문에 Pop하고 Push를 안한거일지도?
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UCm_CameraModeStack::BlendStack(FCm_CameraModeView& OutCameraModeView)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack은 Bottom -> Top 순서로 Blend를 진행한다
	const UCm_CameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	// 참고로 가장 아래 (Bottom)은 BlendWeight가 1.0이다!
	OutCameraModeView = CameraMode->View;

	// 이미 Index = [StackSize - 1] 이미 OutCameraModeView로 지정했으므로, StackSize - 2부터 순회하면 된다
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// HakCameraModeView Blend 함수를 구현하자:
		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}
