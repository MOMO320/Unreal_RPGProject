// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameState.h"

ARPGGameState::ARPGGameState()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 ARPGGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ARPGGameState::AddGameScore()
{
	TotalGameScore++;
}

void ARPGGameState::SetGameCleared()
{
	bGameCleared = true;
}

bool ARPGGameState::IsGameCleared() const
{
	return bGameCleared;
}
