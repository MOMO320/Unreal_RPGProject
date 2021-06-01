// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerState.h"

ARPGPlayerState::ARPGPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
}

int32 ARPGPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ARPGPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void ARPGPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	CharacterLevel = 5;
	GameScore = 0;
}
