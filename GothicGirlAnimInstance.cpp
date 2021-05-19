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

	// 객체가 유효한지 점검하는 함수
	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{	// Size() : 두사이의 거리를 구하는 함수
		CurrentPawnSpeed = Pawn->GetVelocity().Size(); 

		// 객체 유형을 안전하게 동적으로 캐스팅
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}		
	}
}
