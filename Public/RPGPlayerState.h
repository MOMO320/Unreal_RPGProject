// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API ARPGPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ARPGPlayerState();

	int32 GetGameScore() const;
	int32	GetCharacterLevel() const;
	int32 GetGameHighScore() const;
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);

	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

	void AddGameScore();

public:
	FString SaveSlotName;


protected:
	UPROPERTY(Transient)
		int32 GameScore;

	UPROPERTY(Transient)
		int32 CharacterLevel;

	UPROPERTY(Transient)
		int32 Exp;

	UPROPERTY(Transient)
		int32 GameHighScore;

private:
	void SetCharacterLevel(int32 newCharacterLevel);
	struct FGothicCharacterData* CurrentStatData;

};
