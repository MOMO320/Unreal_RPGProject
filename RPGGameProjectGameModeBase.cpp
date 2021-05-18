// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGGameProjectGameModeBase.h"
#include "RPGPawn.h"
#include "GothicCharacter.h"
#include "RPGPlayerController.h"

ARPGGameProjectGameModeBase::ARPGGameProjectGameModeBase()
{
	DefaultPawnClass = AGothicCharacter::StaticClass();
	PlayerControllerClass = ARPGPlayerController::StaticClass();
}