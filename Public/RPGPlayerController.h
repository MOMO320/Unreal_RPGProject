// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARPGPlayerController();

	virtual void PostInitializeComponents() override;
	class UUserHDWidget* GetUserHUDWidget() const;
	void NPCKill(class AGothicCharacter* KilledNPC) const;
	void AddGameScore() const;
	void ChangeInputMode(bool bGameMode = true);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserHDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UGamePlayWidget> MenuWidgetClass;

private:
	void OnGamePause();

private:
	UPROPERTY()
		class UUserHDWidget* HUDWidget;

	UPROPERTY()
		class ARPGPlayerState* RPGPlayerState;

	UPROPERTY()
		class UGamePlayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;

};
