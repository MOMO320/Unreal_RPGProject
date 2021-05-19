// Fill out your copyright notice in the Description page of Project Settings.


#include "GothicGirlAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGothicGirlAnimInstance::UGothicGirlAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
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
