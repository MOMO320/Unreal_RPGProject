// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "NPCAIController.h"
#include "GothicCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto GothicCharacter = Cast<AGothicCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == GothicCharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<AGothicCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - GothicCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	GothicCharacter->SetActorRotation(FMath::RInterpTo(GothicCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
