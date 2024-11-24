// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Cm_Character.h"

#include "Character/Cm_PawnExtensionComponent.h"

// Sets default values
ACm_Character::ACm_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PawnExtComp = CreateDefaultSubobject<UCm_PawnExtensionComponent>(L"PawnExtenstion");
}

void ACm_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComp->SetupPlayerInputComponent();
}

