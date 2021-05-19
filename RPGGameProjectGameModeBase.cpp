// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGGameProjectGameModeBase.h"
#include "RPGPawn.h"
<<<<<<< HEAD
#include "GothicCharacter.h"
=======
>>>>>>> c06ff3c (발키리 캐릭터 기본 생성)
#include "RPGPlayerController.h"

ARPGGameProjectGameModeBase::ARPGGameProjectGameModeBase()
{
<<<<<<< HEAD
	DefaultPawnClass = AGothicCharacter::StaticClass();
=======
	DefaultPawnClass = ARPGPawn::StaticClass();
>>>>>>> c06ff3c (발키리 캐릭터 기본 생성)
	PlayerControllerClass = ARPGPlayerController::StaticClass();
}