#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cm_Character.generated.h"

class UCm_PawnExtensionComponent;

UCLASS()
class LYRACLONE_API ACm_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ACm_Character(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	TObjectPtr<UCm_PawnExtensionComponent> PawnExtComp;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
