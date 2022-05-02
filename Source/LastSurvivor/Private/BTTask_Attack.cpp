// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "LSAIController.h"
#include "LSMob.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ALSMob* Mob = Cast<ALSMob>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == Mob) return EBTNodeResult::Failed;

	Mob->Attack();
	return EBTNodeResult::Succeeded;
}