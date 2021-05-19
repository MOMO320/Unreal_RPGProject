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
	virtual void PostInitializeComponents() override;
	
<<<<<<< HEAD
protected:
	virtual void BeginPlay() override;
=======
>>>>>>> c06ff3c (발키리 캐릭터 기본 생성)
};
