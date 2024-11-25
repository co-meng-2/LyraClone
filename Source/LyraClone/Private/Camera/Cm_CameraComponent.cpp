// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Cm_CameraComponent.h"

#include "Camera/Cm_CameraMode.h"

UCm_CameraComponent::UCm_CameraComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UCm_CameraComponent::OnRegister()
{
	Super::OnRegister();

	if(!CameraModeStack)
	{
		CameraModeStack = NewObject<UCm_CameraModeStack>(this);
	}
}

void UCm_CameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	// 부모 구현 무시
	// Super::GetCameraView(DeltaTime, DesiredView);

	// CammeraModeStack에 전달된 CameraModeClass를 Push
	UpdateCameraModes();

	
	FCm_CameraModeView Loc_CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, Loc_CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			// Controller 방향을 Pawn방향으로 바꿔줌.
			PC->SetControlRotation(Loc_CameraModeView.ControlRotation);
		}
	}
	
	SetWorldLocationAndRotation(Loc_CameraModeView.Location, Loc_CameraModeView.Rotation);
	FieldOfView = Loc_CameraModeView.FieldOfView;

	// 최종 View 설정
	DesiredView.Location = Loc_CameraModeView.Location;
	DesiredView.Rotation = Loc_CameraModeView.Rotation;
	DesiredView.FOV = Loc_CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void UCm_CameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);
	
	if(DetermineCameraModeDelegate.IsBound())
	{
		// PawnData에서 CameraModeClass를 전달하는 Delegate로 등록되어 있다.
		// HeroComponent의 HandleInitStateChagned에서 Binding된다.
		if(TSubclassOf<UCm_CameraMode> Loc_CameraModeClass{DetermineCameraModeDelegate.Execute()})
		{
			CameraModeStack->PushCameraMode(Loc_CameraModeClass);
		}
	}
}
