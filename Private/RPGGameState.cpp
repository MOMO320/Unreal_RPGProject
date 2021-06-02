// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameState.h"

ARPGGameState::ARPGGameState()
{
	TotalGameScore = 0;
}

int32 ARPGGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ARPGGameState::AddGameScore()
{
	TotalGameScore++;
}
