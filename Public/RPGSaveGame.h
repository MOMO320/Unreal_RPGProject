// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "GameFramework/SaveGame.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	URPGSaveGame();

public:

	UPROPERTY()
		int32 Level;

	UPROPERTY()
		int32 Exp;

	UPROPERTY()
		FString PlayerName;

	UPROPERTY()
		int32 HighScore;

};
