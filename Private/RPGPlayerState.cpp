// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerState.h"
#include "RPGGameInstance.h"
#include "RPGSaveGame.h"

ARPGPlayerState::ARPGPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	CharacterIndex = 0;
	SaveSlotName = TEXT("Player1");
}

int32 ARPGPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ARPGPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 ARPGPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 ARPGPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
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
	SavePlayerData();
	return DidLevelUp;
}

void ARPGPlayerState::InitPlayerData()
{
	auto RPGSaveGame = Cast<URPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	if (nullptr == RPGSaveGame)
	{
		RPGSaveGame = GetMutableDefault<URPGSaveGame>();
	}
	
	SetPlayerName(RPGSaveGame->PlayerName);
	SetCharacterLevel(RPGSaveGame->Level);
	GameScore = 0;
	GameHighScore = RPGSaveGame->HighScore;
	Exp = RPGSaveGame->Exp;
	CharacterIndex = RPGSaveGame->CharacterIndex;
	SavePlayerData();
}

void ARPGPlayerState::SavePlayerData()
{
	URPGSaveGame* NewPlayerData = NewObject<URPGSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}

void ARPGPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void ARPGPlayerState::SetCharacterLevel(int32 newCharacterLevel)
{
	auto RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
	ABCHECK(nullptr != RPGGameInstance);

	CurrentStatData = RPGGameInstance->GetGothicCharacterData(newCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);

	CharacterLevel = newCharacterLevel;
}
