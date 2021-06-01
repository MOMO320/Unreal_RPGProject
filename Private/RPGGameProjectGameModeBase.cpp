// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGGameProjectGameModeBase.h"
#include "RPGPawn.h"
#include "GothicCharacter.h"
#include "RPGPlayerController.h"
#include "RPGPlayerState.h"

ARPGGameProjectGameModeBase::ARPGGameProjectGameModeBase()
{
	DefaultPawnClass = AGothicCharacter::StaticClass();
	PlayerControllerClass = ARPGPlayerController::StaticClass();
	PlayerStateClass = ARPGPlayerState::StaticClass();
}

void ARPGGameProjectGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto RPGPlayerState = Cast<ARPGPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != RPGPlayerState);
	RPGPlayerState->InitPlayerData();
}
