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
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		ABLOG(Error, TEXT("Level(%d) data doesn't exist"), NewLevel);
	}
}

void UGothicChracterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UGothicChracterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();

	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

int32 UGothicChracterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

float UGothicChracterStatComponent::GetAttack() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float UGothicChracterStatComponent::GetHPRatio() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER)? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
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

