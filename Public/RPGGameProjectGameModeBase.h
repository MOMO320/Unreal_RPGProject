// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API ARPGGameProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public :
	ARPGGameProjectGameModeBase();

	virtual void PostInitializeComponents() override;
	virtual  void PostLogin(APlayerController* NewPlayer) override;
	void AddScore(class ARPGPlayerController* ScoredPlayer);
	int32 GetScore() const;

private:
	UPROPERTY()
		class ARPGGameState* RPGGameState;

};
