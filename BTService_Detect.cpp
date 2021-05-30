// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "NPCAIController.h"
#include "GothicCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr == ControllingPawn)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World)
		return;

	TArray<FOverlapResult> OverlapResults;
	// 충돌 함수로 전달된 매개 변수를 정의하는 구조
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, nullptr);

		for (auto OverlapResult : OverlapResults)
		{
			AGothicCharacter* GothicCharacter = Cast<AGothicCharacter>(OverlapResult.GetActor());
			
			if (GothicCharacter && GothicCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, GothicCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, GothicCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), GothicCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				UE_LOG(LogTemp, Warning, TEXT(" Find! "));
				return;
			}
		}
	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
