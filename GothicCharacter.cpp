// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicCharacter.h"
#include "GothicChracterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "GothicWeapon.h"
#include "GothicGirlAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "GothicCharacterWidget.h"
#include "NPCAIcontroller.h"

// Sets default values
AGothicCharacter::AGothicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	GothicCharacterStat = CreateDefaultSubobject<UGothicChracterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPARWIDGET"));
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("GothicCharacter"));
	
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurrentControlMode = EControlMode::GTA;

	ConstructorFinder();
	SetControlMode(CurrentControlMode);

	isJumpStart = false;
	IsAttacking = false;

	MaxCombo = 4;

	AttackEndComboState();

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

}

// Called when the game starts or when spawned
void AGothicCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto CharacterWidget = Cast<UGothicCharacterWidget>(HPBarWidget->GetUserWidgetObject());

	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharaceterStat(GothicCharacterStat);
	}

}

void AGothicCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;
	
	switch (CurrentControlMode)
	{
	case EControlMode::GTA :

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
		break;

	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;

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

	GothicGirlAnim->OnAttackHitCheck.AddUObject(this, &AGothicCharacter::AttackCheck);

	GothicCharacterStat->OnHPIsZero.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		GothicGirlAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
}

void AGothicCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::GTA);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

float AGothicCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ABLOG(Error, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	
	GothicCharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

bool AGothicCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AGothicCharacter::SetWeapon(AGothicWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("WeaponSocket"));

	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AGothicCharacter::ConstructorFinder()
{
	// Widget Finder
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));

		if (UI_HUD.Succeeded())
		{
			HPBarWidget->SetWidgetClass(UI_HUD.Class);
			HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
		}
	}

	// SkeletalMesh Finder
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOTHIC_GIRL(TEXT("/Game/GothicGirl/Character/Meshes/Skin01/SK_GothicGirl_NPRSkin.SK_GothicGirl_NPRSkin"));

		if (SK_GOTHIC_GIRL.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(SK_GOTHIC_GIRL.Object);
		}

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	// AnimInstance Finder
	{
		static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_GOTHIC_GIRL(TEXT("/Game/BluePrints/BP_GothicAnim.BP_GothicAnim_C"));

		if (ANIM_GOTHIC_GIRL.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(ANIM_GOTHIC_GIRL.Class);
		}
	}

	// Socket Mesh Finder
	{
		FName BeltSocket(TEXT("BeltSocket"));

		if (GetMesh()->DoesSocketExist(BeltSocket))
		{
			Belt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BELT"));
			static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BELT(TEXT("/Game/GothicGirl/Weapon/Meshes/SK_Belt.SK_Belt"));
			if (SK_BELT.Succeeded())
			{
				Belt->SetSkeletalMesh(SK_BELT.Object);
			}

			Belt->SetupAttachment(GetMesh(), BeltSocket);
		}
	}
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

void AGothicCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();	// 쿼터니언
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif 

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GothicCharacterStat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}



