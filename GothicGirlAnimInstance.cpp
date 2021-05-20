// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicGirlAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGothicGirlAnimInstance::UGothicGirlAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;

	// AnimMontage Finder
	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/BluePrints/SK_GothicGirl_Attack.SK_GothicGirl_Attack"));

		if (ATTACK_MONTAGE.Succeeded())
		{
			AttackMontage = ATTACK_MONTAGE.Object;
		}
	}
}

void UGothicGirlAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ��ü�� ��ȿ���� �����ϴ� �Լ�
	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{	// Size() : �λ����� �Ÿ��� ���ϴ� �Լ�
		CurrentPawnSpeed = Pawn->GetVelocity().Size(); 

		// ��ü ������ �����ϰ� �������� ĳ����
		auto Character = Cast<ACharacter>(Pawn);

		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}				
	}
}


void UGothicGirlAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UGothicGirlAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UGothicGirlAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UGothicGirlAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UGothicGirlAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
