// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicCharacterWidget.h"
#include "GothicChracterStatComponent.h"
#include "Components/ProgressBar.h"


void UGothicCharacterWidget::BindCharaceterStat(class UGothicChracterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	
	NewCharacterStat->OnHPChanged.AddUObject(this, &UGothicCharacterWidget::UpdateHPWidget);
}

void UGothicCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UGothicCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
