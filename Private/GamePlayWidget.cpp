// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "Components/Button.h"
#include "RPGPlayerController.h"

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UGamePlayWidget::OnRetrygameClicked);
	}
}

void UGamePlayWidget::OnResumeClicked()
{
	auto PlayerController = Cast<ARPGPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != PlayerController);

	RemoveFromParent();
	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);
}

void UGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UGamePlayWidget::OnRetrygameClicked()
{
	auto RPGPlayerController = Cast<ARPGPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != RPGPlayerController);
	RPGPlayerController->RestartLevel();
}
