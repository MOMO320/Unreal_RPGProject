// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicWeapon.h"

// Sets default values
AGothicWeapon::AGothicWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/GothicGirl/Weapon/Meshes/SK_Sword.SK_Sword"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	AttackRange = 150.0f;
	AttackDamageMin = -2.5f;
	AttackDamageMax = 10.0f;
	AttackModifierMin = 0.85f;
	AttackModifierMax = 1.25f;
}

// Called when the game starts or when spawned
void AGothicWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);

	ABLOG(Warning, TEXT("Weapon Damage : %f , Modifier : %f"), AttackDamage, AttackModifier);
}

// Called every frame
void AGothicWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AGothicWeapon::GetAttackRange() const
{
	return AttackRange;
}

float AGothicWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

float AGothicWeapon::GetAttackModifier() const
{
	return AttackModifier;
}

