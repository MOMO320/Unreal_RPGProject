// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrol.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAMEPROJECT_API UBTTask_FindPatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
		
};
