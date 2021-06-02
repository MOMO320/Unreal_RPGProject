// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "UserHDWidget.h"
#include "RPGPlayerState.h"
#include "GothicCharacter.h"
#include "GamePlayWidget.h"

ARPGPlayerController::ARPGPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserHDWidget> UI_HUD_C(TEXT("/Game/UI/UI_HUD.UI_HUD_C"));

	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UGamePlayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));

	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}
}

void ARPGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

UUserHDWidget* ARPGPlayerController::GetUserHUDWidget() const
{
	return HUDWidget;
}

void ARPGPlayerController::NPCKill(AGothicCharacter* KilledNPC) const
{
	RPGPlayerState->AddExp(KilledNPC->GetExp());
}

void ARPGPlayerController::AddGameScore() const
{
	RPGPlayerState->AddGameScore();
}

void ARPGPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
		
	ChangeInputMode(true);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UUserHDWidget>(this, HUDWidgetClass);
	ABCHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	RPGPlayerState = Cast<ARPGPlayerState>(PlayerState);
	ABCHECK(nullptr != RPGPlayerState);
	HUDWidget->BindPlayerState(RPGPlayerState);
	RPGPlayerState->OnPlayerStateChanged.Broadcast();

}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &ARPGPlayerController::OnGamePause);
}

void ARPGPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UGamePlayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}
