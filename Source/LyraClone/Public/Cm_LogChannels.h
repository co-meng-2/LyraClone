// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"


// DECLARE_LOG_CATEGORY_EXTERN(CmLog, Log, All);
extern struct FLogCategoryLogCm : public FLogCategory<ELogVerbosity::Log, ELogVerbosity::All>
{ FORCEINLINE FLogCategoryLogCm() : FLogCategory(TEXT("LogCm")) {} } LogCm;;
											// 이 부분에서 전역struct 객체를 만들어주고 있음.

#define CM_LOG(Message, ...) UE_LOG(LogCm, Warning, TEXT(Message), ##__VA_ARGS__)
