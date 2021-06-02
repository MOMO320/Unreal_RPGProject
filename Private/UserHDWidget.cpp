// Fill out your copyright notice in the Description page of Project Settings.


#include "UserHDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GothicChracterStatComponent.h"
#include "RPGPlayerState.h"


void UUserHDWidget::BindCharacterStat(UGothicChracterStatComponent* CharacterStat)
{
	ABCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UUserHDWidget::UpdateCharacterStat);
}

void UUserHDWidget::BindPlayerState(ARPGPlayerState* PlayerState)
{
	ABCHECK(nullptr != PlayerState);
	CurrentRPGPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UUserHDWidget::UpdatePlayerState);
}

void UUserHDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
		
	ABCHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	ABCHECK(nullptr != ExpBar);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	ABCHECK(nullptr != PlayerLevel);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	ABCHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	ABCHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	ABCHECK(nullptr != HighScore);
}

void UUserHDWidget::UpdateCharacterStat()
{
	ABCHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void UUserHDWidget::UpdatePlayerState()
{
	ABCHECK(CurrentRPGPlayerState.IsValid());

	ExpBar->SetPercent(CurrentRPGPlayerState->GetExpRatio());
	PlayerName->SetText(FText::FromString(CurrentRPGPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentRPGPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentRPGPlayerState->GetGameScore())));
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentRPGPlayerState->GetGameHighScore())));
}
