// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraClone.h"

#include "Cm_LogChannels.h"
#include "Modules/ModuleManager.h"

class FLyraCloneGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		FDefaultGameModuleImpl::StartupModule();
		UE_LOG(LogCm, Warning, L"LyraCloneModuleStarted!");
	}
	virtual void ShutdownModule() override
	{
		FDefaultGameModuleImpl::ShutdownModule();
	}
};


IMPLEMENT_PRIMARY_GAME_MODULE( FLyraCloneGameModule, LyraClone, "LyraClone" );
