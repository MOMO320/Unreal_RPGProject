// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "GameFramework/GameStateBase.h"
#include "RPGGameState.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API ARPGGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ARPGGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();

private:
	UPROPERTY(Transient)
		int32 TotalGameScore;

};
