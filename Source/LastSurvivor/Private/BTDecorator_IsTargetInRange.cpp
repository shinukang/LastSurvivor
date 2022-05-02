
#include "BTDecorator_IsTargetInRange.h"
#include "LSAIController.h"
#include "LSCharacter.h"
#include "LSMob.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsTargetInRange::UBTDecorator_IsTargetInRange()
{
	NodeName = TEXT("IsTargetInRange");
}

bool UBTDecorator_IsTargetInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ALSMob* Mob = Cast<ALSMob>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == Mob) return false;

	const ALSCharacter* Target = Cast<ALSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALSAIController::TargetKey));
	if(nullptr == Target) return false;

	const bool Result = Target->GetDistanceTo(Mob) <= Mob->GetAttackRange();

	return Result;
}