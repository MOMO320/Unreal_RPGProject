// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGGameProjectGameModeBase.h"
#include "RPGPawn.h"
#include "RPGPlayerController.h"

ARPGGameProjectGameModeBase::ARPGGameProjectGameModeBase()
{
	DefaultPawnClass = ARPGPawn::StaticClass();
	PlayerControllerClass = ARPGPlayerController::StaticClass();
}