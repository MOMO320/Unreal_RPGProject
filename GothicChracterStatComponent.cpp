// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicChracterStatComponent.h"
#include "RPGGameInstance.h"

// Sets default values for this component's properties
UGothicChracterStatComponent::UGothicChracterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
	// ...
}


void UGothicChracterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto GothicInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != GothicInstance);

	CurrentStatData = GothicInstance->GetGothicCharacterData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;
	}
	else
	{
		ABLOG(Error, TEXT("Level(%d) data doesn't exist"), NewLevel);
	}
}

// Called when the game starts
void UGothicChracterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGothicChracterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

