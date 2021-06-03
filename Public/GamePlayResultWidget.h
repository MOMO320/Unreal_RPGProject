// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "GamePlayWidget.h"
#include "GamePlayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API UGamePlayResultWidget : public UGamePlayWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState(class ARPGGameState* GameState);

protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class ARPGGameState> CurrentGameState;
};
