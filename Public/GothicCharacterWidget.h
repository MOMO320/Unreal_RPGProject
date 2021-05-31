// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "Blueprint/UserWidget.h"
#include "GothicCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API UGothicCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharaceterStat(class UGothicChracterStatComponent* NewCharacterStat);


protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UGothicChracterStatComponent> CurrentCharacterStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
