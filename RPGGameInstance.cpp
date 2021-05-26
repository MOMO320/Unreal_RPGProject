// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"

URPGGameInstance::URPGGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/GothicCharacterData.GothicCharacterData");
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_GOTHICCHARACTER(*CharacterDataPath);

	ABCHECK(DT_GOTHICCHARACTER.Succeeded());
	GothicCharacterTabel = DT_GOTHICCHARACTER.Object;

	ABCHECK(GothicCharacterTabel->GetRowMap().Num() > 0);
}

void URPGGameInstance::Init()
{
	Super::Init();


}

FGothicCharacterData* URPGGameInstance::GetGothicCharacterData(int32 Level)
{
	return GothicCharacterTabel->FindRow<FGothicCharacterData>(*FString::FromInt(Level), TEXT(""));
}
