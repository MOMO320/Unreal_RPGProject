// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "NPCAIController.h"
#include "GothicCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto GothicCharacter = Cast<AGothicCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == GothicCharacter)
		return EBTNodeResult::Failed;

	GothicCharacter->Attack();
	IsAttacking = true;
	GothicCharacter->OnAttackEnd.AddLambda([this]()-> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
