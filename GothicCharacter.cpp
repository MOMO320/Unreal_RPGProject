// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicCharacter.h"
#include "GothicGirlAnimInstance.h"

// Sets default values
AGothicCharacter::AGothicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	// SkeletalMesh Finder
	{	
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOTHIC_GIRL(TEXT("/Game/GothicGirl/Character/Meshes/SK_GothicGirl.SK_GothicGirl"));

		if (SK_GOTHIC_GIRL.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(SK_GOTHIC_GIRL.Object);
		}

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	// AnimInstance Finder
	{
		static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_GOTHIC_GIRL(TEXT("/Game/BluePrints/GothicAnimBluePrint.GothicAnimBluePrint_C"));
		
		if (ANIM_GOTHIC_GIRL.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(ANIM_GOTHIC_GIRL.Class);
		}
	}

	SetControlMode(0);

	isJumpStart = false;
	IsAttacking = false;

	MaxCombo = 4;
	AttackEndComboState();

}

// Called when the game starts or when spawned
void AGothicCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGothicCharacter::SetControlMode(int32 ControlMode)
{
	if (ControlMode == 0)
	{
		SpringArm->TargetArmLength = 450.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		GetCharacterMovement()->JumpZVelocity = 600.0f;
	}
}

// Called every frame  
void AGothicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGothicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 이벤트 셋업
	{
		PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AGothicCharacter::UpDown);
		PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AGothicCharacter::LeftRight);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AGothicCharacter::LookUp);
		PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AGothicCharacter::Turn);
		PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AGothicCharacter::Jump);
		PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AGothicCharacter::Attack);
	}
}

void AGothicCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GothicGirlAnim = Cast<UGothicGirlAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != GothicGirlAnim);

	GothicGirlAnim->OnMontageEnded.AddDynamic(this, &AGothicCharacter::OnAttackMontageEnded);
	GothicGirlAnim->OnNextAttackCheck.AddLambda([this]()->void {
	
		ABLOG(Warning, TEXT("On Next Attack Check"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			GothicGirlAnim->JumpToAttackMontageSection(CurrentCombo);
		}

	});
}

void AGothicCharacter::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X),NewAxisValue);
}

void AGothicCharacter::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AGothicCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AGothicCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AGothicCharacter::Attack()
{
	
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		GothicGirlAnim->PlayAttackMontage();
		GothicGirlAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}	
}

void AGothicCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking =false;
	AttackEndComboState();
}

void AGothicCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0 , MaxCombo -1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AGothicCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}



