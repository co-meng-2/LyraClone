// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Cm_Character.h"

// Sets default values
ACm_Character::ACm_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACm_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACm_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACm_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

