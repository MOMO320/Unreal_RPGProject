// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerState.h"
#include "RPGGameInstance.h"

ARPGPlayerState::ARPGPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
}

int32 ARPGPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ARPGPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

float ARPGPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	ABLOG(Warning, TEXT("Ratio : %f , Current : %d , Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool ARPGPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	return DidLevelUp;
}

void ARPGPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("°íµñ_GIRL"));
	SetCharacterLevel(5);
	CharacterLevel = 5;
	GameScore = 0;
	Exp = 0;
}

void ARPGPlayerState::AddGameScore()
{
	GameScore++;
	OnPlayerStateChanged.Broadcast();
}

void ARPGPlayerState::SetCharacterLevel(int32 newCharacterLevel)
{
	auto RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
	ABCHECK(nullptr != RPGGameInstance);

	CurrentStatData = RPGGameInstance->GetGothicCharacterData(newCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = newCharacterLevel;
}
