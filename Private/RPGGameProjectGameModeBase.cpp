// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGGameProjectGameModeBase.h"
#include "RPGPawn.h"
#include "GothicCharacter.h"
#include "RPGPlayerController.h"
#include "RPGPlayerState.h"
#include "RPGGameState.h"

ARPGGameProjectGameModeBase::ARPGGameProjectGameModeBase()
{
	DefaultPawnClass = AGothicCharacter::StaticClass();
	PlayerControllerClass = ARPGPlayerController::StaticClass();
	PlayerStateClass = ARPGPlayerState::StaticClass();
	GameStateClass = ARPGGameState::StaticClass();
	ScoreToClear = 2;
}

void ARPGGameProjectGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RPGGameState = Cast<ARPGGameState>(GameState);
}

void ARPGGameProjectGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto RPGPlayerState = Cast<ARPGPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != RPGPlayerState);
	RPGPlayerState->InitPlayerData();
}

void ARPGGameProjectGameModeBase::AddScore(ARPGPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto RPGPlayerController = Cast<ARPGPlayerController>(It->Get());
		if ((nullptr != RPGPlayerController) && (ScoredPlayer == RPGPlayerController))
		{
			RPGPlayerController->AddGameScore();
			break;
		}
	}

	RPGGameState->AddGameScore();

	if (GetScore() >= ScoreToClear)
	{
		RPGGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto RPGPlayerController = Cast<ARPGPlayerController>(It->Get());
			if (nullptr != RPGPlayerController)
			{
				RPGPlayerController->ShowResultUI();
			}
		}
	}
}

int32 ARPGGameProjectGameModeBase::GetScore() const
{
	return RPGGameState->GetTotalGameScore();
}
