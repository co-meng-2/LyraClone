// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cm_InputConfig.h"
#include "EnhancedInputComponent.h"
#include "Cm_EnhancedInputComponent.generated.h"

class UCm_InputConfig;
/**
 * 
 */
UCLASS()
class LYRACLONE_API UCm_EnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UCm_EnhancedInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	template <class UserClass, typename FuncType>
	void BindNativeAction(const UCm_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);
};

template <class UserClass, typename FuncType>
void UCm_EnhancedInputComponent::BindNativeAction(const UCm_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	// 여기서 알 수 있듯이, InputConfig는 활성화 가능한 InputAction을 담고 있다.
	// - 만약 InputConfig에 없는 InputAction을 Binding시키면, nullptr을 반환하여, 바인딩하는데 실패한다!
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}
