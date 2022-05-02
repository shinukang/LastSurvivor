#include "BTTask_Patrol.h"
#include "LSAIController.h"
#include "LSMob.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == Pawn) return EBTNodeResult::Failed;

	const ALSMob* Mob = Cast<ALSMob>(Pawn);
	if(nullptr == Mob)  return EBTNodeResult::Failed;
	
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
	if(nullptr == NavSystem) return EBTNodeResult::Failed;
	
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 300.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ALSAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
