// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ANPCAIController::HomePosKey(TEXT("HomePos"));
const FName ANPCAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ANPCAIController::TargetKey(TEXT("Target"));

ANPCAIController::ANPCAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_GothicCharacter.BB_GothicCharacter"));
	
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_GothicCharacter.BT_GothicCharacter"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//if (UseBlackboard(BBAsset, Blackboard))
	//{
	//	Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
	//	if (!RunBehaviorTree(BTAsset))
	//	{
	//		ABLOG(Error, TEXT("AIController couldn't run behaviour tree!"));
	//	}
	//}
}

void ANPCAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ANPCAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

