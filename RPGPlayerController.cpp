// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"

void ARPGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
