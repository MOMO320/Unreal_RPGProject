// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "UserHDWidget.h"
#include "RPGPlayerState.h"
#include "GothicCharacter.h"

ARPGPlayerController::ARPGPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserHDWidget> UI_HUD_C(TEXT("/Game/UI/UI_HUD.UI_HUD_C"));

	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
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

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UUserHDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();


	RPGPlayerState = Cast<ARPGPlayerState>(PlayerState);
	ABCHECK(nullptr != RPGPlayerState);
	HUDWidget->BindPlayerState(RPGPlayerState);
	RPGPlayerState->OnPlayerStateChanged.Broadcast();

}

void ARPGPlayerController::SetupInputComponent()
{
}
