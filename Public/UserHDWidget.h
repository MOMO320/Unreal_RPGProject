// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "Blueprint/UserWidget.h"
#include "UserHDWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API UUserHDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UGothicChracterStatComponent* CharacterStat);
	void BindPlayerState(class ARPGPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UGothicChracterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ARPGPlayerState> CurrentRPGPlayerState;
	
	UPROPERTY()
		class UProgressBar* HPBar;

	UPROPERTY()
		class UProgressBar* ExpBar;

	UPROPERTY()
		class UTextBlock* PlayerName;

	UPROPERTY()
		class UTextBlock* PlayerLevel;

	UPROPERTY()
		class UTextBlock* CurrentScore;

	UPROPERTY()
		class UTextBlock* HighScore;
};
