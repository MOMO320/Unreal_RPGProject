// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPawn.h"

// Sets default values
ARPGPawn::ARPGPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_VALKIRY(TEXT("/Game/GothicGirl/Character/Meshes/SK_GothicGirl.SK_GothicGirl"));

	if (SK_VALKIRY.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_VALKIRY.Object);
	}

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> GOTHIC_ANIM(TEXT("/Game/BluePrints/GothicAnimBluePrint.GothicAnimBluePrint_C"));

	if (GOTHIC_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(GOTHIC_ANIM.Class);
	}
}

// Called when the game starts or when spawned
void ARPGPawn::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ARPGPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ARPGPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ARPGPawn::LeftRight);

}

void ARPGPawn::UpDown(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void ARPGPawn::LeftRight(float NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

