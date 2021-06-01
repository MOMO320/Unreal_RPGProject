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

	void InitPlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
		int32 GameScore;

	UPROPERTY(Transient)
		int32 CharacterLevel;
};