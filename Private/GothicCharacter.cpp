// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicCharacter.h"
#include "GothicChracterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "GothicWeapon.h"
#include "GothicGirlAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "GothicCharacterWidget.h"
#include "NPCAIcontroller.h"
#include "GothicCharacterSetting.h"
#include "RPGPlayerController.h"
#include "RPGGameInstance.h"
#include "RPGPlayerState.h"
#include "UserHDWidget.h"
#include "RPGGameProjectGameModeBase.h"

// Sets default values
AGothicCharacter::AGothicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto DefaultSetting = GetDefault<UGothicCharacterSetting>();

	//ABLOG(Error, TEXT("Character Asset Count : %s"), DefaultSetting->CharacterAssets.Num());
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}

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

	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	AssetIndex = 1;

	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);

	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void AGothicCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		GothicCharacterController = Cast<ARPGPlayerController>(GetController());
		ABCHECK(nullptr != GothicCharacterController);
	}
	else
	{
		NPCAIController = Cast<ANPCAIController>(GetController());
		ABCHECK(nullptr != NPCAIController);
	}

	auto DefaultSetting = GetDefault<UGothicCharacterSetting>();

	if (bIsPlayer)
	{
		auto RPGPlayerState = Cast<ARPGPlayerState>(GetPlayerState());
		ABCHECK(nullptr != RPGPlayerState);
		AssetIndex = RPGPlayerState->GetCharacterIndex();
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto RpgGameInstance = Cast<URPGGameInstance>(GetGameInstance());
	ABCHECK(nullptr != RpgGameInstance);
	AssetStreamingHandle = RpgGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AGothicCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);

	// 주석
	{
		//if (!IsPlayerControlled())
		//{
		//	auto DefaultSetting = GetDefault<UGothicCharacterSetting>();
		//	int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		//
		//	CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];
		//
		//	auto RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
		//
		//	if (nullptr != RPGGameInstance)
		//	{
		//		AssetStreamingHandle = RPGGameInstance->StreamableManager.RequestAsyncLoad(
		//			CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AGothicCharacter::OnAssetLoadCompleted));
		//	}
		//}
		//
		//auto CharacterWidget = Cast<UGothicCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		//
		//if (nullptr != CharacterWidget)
		//{
		//	CharacterWidget->BindCharaceterStat(GothicCharacterStat);
		//}
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

	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto RPGPlayerController = Cast<ARPGPlayerController>(EventInstigator);
			ABCHECK(nullptr != RPGPlayerController, 0.0f);
			RPGPlayerController->NPCKill(this);
		}
	}

	return FinalDamage;
}

bool AGothicCharacter::CanSetWeapon()
{
	return true;
}

void AGothicCharacter::SetWeapon(AGothicWeapon* NewWeapon)
{

	ABCHECK(nullptr != NewWeapon);
	
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
	
	FName WeaponSocket(TEXT("WeaponSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AGothicCharacter::SetCharacterState(ECharacterState NewState)
{
	ABCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(GothicCharacterController);

			GothicCharacterController->GetUserHUDWidget()->BindCharacterStat(GothicCharacterStat);

			auto RPGPlayerState = Cast<ARPGPlayerState>(GetPlayerState());
			ABCHECK(nullptr != RPGPlayerState);
			GothicCharacterStat->SetNewLevel(RPGPlayerState->GetCharacterLevel());

		}
		else
		{
			auto RPGProjectGameMode = Cast<ARPGGameProjectGameModeBase>(GetWorld()->GetAuthGameMode());
			ABCHECK(nullptr != RPGProjectGameMode);
			int32 TargetLevel = FMath::CeilToInt(((float)RPGProjectGameMode->GetScore() * 0.8f));
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			ABLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			GothicCharacterStat->SetNewLevel(FinalLevel);
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}

	case ECharacterState::READY:
	{

		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		GothicCharacterStat->OnHPIsZero.AddLambda([this]()-> void {
			SetCharacterState(ECharacterState::DEAD);
		});

		auto GothicCharacterWidget = Cast<UGothicCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		ABCHECK(nullptr != GothicCharacterWidget);
		GothicCharacterWidget->BindCharaceterStat(GothicCharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::GTA);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(GothicCharacterController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			NPCAIController->RunAI();
		}

		break;
	}

	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		
		if (bIsPlayer)
		{
			DisableInput(GothicCharacterController);
		}
		else
		{
			NPCAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeatTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			
			if (bIsPlayer)
			{
				GothicCharacterController->ShowResultUI();
			}
			else
			{
				Destroy();
			}}), DeadTimer, false);

		break;
	}
	}
}


ECharacterState AGothicCharacter::GetCharacterState() const
{
	return CurrentState;
}

int32 AGothicCharacter::GetExp() const
{
	return GothicCharacterStat->GetDropExp();
}

float AGothicCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon)? CurrentWeapon->GetAttackRange() : AttackRange;
}

float AGothicCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? (GothicCharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : GothicCharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;

	return AttackDamage * AttackModifier;
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

void AGothicCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());

	AssetStreamingHandle.Reset();

	ABCHECK(nullptr != AssetLoaded);

	GetMesh()->SetSkeletalMesh(AssetLoaded);
	
	SetCharacterState(ECharacterState::READY);
	
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
	OnAttackEnd.Broadcast();
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
	float FinalAttackRange = GetFinalAttackRange();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(FinalAttackRange),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
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
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}



