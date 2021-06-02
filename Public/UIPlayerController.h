// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "GameFramework/PlayerController.h"
#include "UIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API AUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget>UIWidgetClass;

	UPROPERTY()
		class UUserWidget* UIWidgetInstance;

};
