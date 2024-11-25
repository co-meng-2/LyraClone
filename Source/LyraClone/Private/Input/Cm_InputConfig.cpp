// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/Cm_InputConfig.h"

#include "Cm_LogChannels.h"

UCm_InputConfig::UCm_InputConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UInputAction* UCm_InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FCm_InputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		CM_LOG("can't find NativeInputAction for InputTag [%s] on InputConfig [%s].", *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
