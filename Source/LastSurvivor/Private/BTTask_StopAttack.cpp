#include "BTTask_StopAttack.h"
#include "LSMob.h"

UBTTask_StopAttack::UBTTask_StopAttack()
{
	NodeName = TEXT("StopAttack");
}

EBTNodeResult::Type UBTTask_StopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ALSMob* Mob = Cast<ALSMob>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == Mob) return EBTNodeResult::Failed;

	Mob->StopAttack();
	
	return EBTNodeResult::Succeeded;
}