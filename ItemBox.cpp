// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "GothicWeapon.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_BOX(TEXT("/Game/Stylized_Assets_Vol1/Meshes/Box_01/Box_01.Box_01"));
	if (SK_BOX.Succeeded())
	{
		Box->SetStaticMesh(SK_BOX.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}


	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));    
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AGothicWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnCharacterOverlap);
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto GothicCharacter = Cast<AGothicCharacter>(OtherActor);

	ABCHECK(nullptr != GothicCharacter);

	if (nullptr != GothicCharacter && nullptr != WeaponItemClass)
	{
		if (GothicCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AGothicWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			GothicCharacter->SetWeapon(NewWeapon);
			
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &AItemBox::OnEffectFinished);
		
		}
		else
		{
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *GothicCharacter->GetName());
		}
	}
}

void AItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}

