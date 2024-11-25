// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cm_CameraComponent.h"
#include "Cm_CameraMode.generated.h"

// Blend 조절 옵션
UENUM(BlueprintType)
enum class ECm_CameraModeBlendFunction : uint8
{
	Linear,
	
	// EaseIn/Out은 exponent 값에 의해 조절된다:
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

// Camera의 상태
struct FCm_CameraModeView
{
	FCm_CameraModeView();

	void Blend(const FCm_CameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;			// CameraRotation
	FRotator ControlRotation;   // PawnRotation
	float FieldOfView;
};

// 
UCLASS(Abstract, NotBlueprintable)
class LYRACLONE_API UCm_CameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	UCm_CameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FCm_CameraModeView View;

	UPROPERTY(EditDefaultsOnly, Category= "Cm|View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;
	UPROPERTY(EditDefaultsOnly, Category= "Cm|View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;
	UPROPERTY(EditDefaultsOnly, Category= "Cm|View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category="Cm|Blend")
	float BlendTime;
	float BlendAlpha;
	float BlendWeight;

	UPROPERTY(EditDefaultsOnly, Category="Cm|Blend")
	float BlendExponent;

	UPROPERTY(EditDefaultsOnly, Category="Cm|Blend")
	ECm_CameraModeBlendFunction BlendFunction;
	
	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);
	
	FVector GetPivotLocation();
	FRotator GetPivotRotation();
	AActor* GetTargetActor();
	const UCm_CameraComponent* GetCmCameraComponent();
};


UCLASS()
class LYRACLONE_API UCm_CameraModeStack : public UObject
{
	GENERATED_BODY()
	
public:
	UCm_CameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void PushCameraMode(TSubclassOf<UCm_CameraMode>& InCameraModeClass);
	UCm_CameraMode* GetCameraModeInstance(TSubclassOf<UCm_CameraMode>& InCameraModeClass);
	void EvaluateStack(float DeltaTime, FCm_CameraModeView& OutCameraModeView);

	void UpdateStack(float DeltaTime);
	void BlendStack(FCm_CameraModeView& OutCameraModeView);
	
	// cache
	UPROPERTY()
	TArray<TObjectPtr<UCm_CameraMode>> CameraModeInstances;
	// 앞에 있을 수록 최근에 추가된 CameraMode
	UPROPERTY()
	TArray<TObjectPtr<UCm_CameraMode>> CameraModeStack;
	
};