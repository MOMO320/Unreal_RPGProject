// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGameProject.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "RPGGameInstance.generated.h"

/**
 * 
 */

// Data Table 
USTRUCT(BlueprintType)
struct FGothicCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGothicCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;
};

UCLASS()
class RPGGAMEPROJECT_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	URPGGameInstance();

	virtual void Init() override;
	FGothicCharacterData* GetGothicCharacterData(int32 Level);

	FStreamableManager StreamableManager;

private:
	UPROPERTY()
		class UDataTable* GothicCharacterTabel;
};
