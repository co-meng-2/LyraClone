#pragma once

#include "CoreMinimal.h"
#include "Cm_GameInstance.generated.h"

UCLASS()
class UCm_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 반드시 ProjectSetting에서 이 클래스로 바꿔줄 것!
	virtual void Init() override;
	virtual void Shutdown() override;
};