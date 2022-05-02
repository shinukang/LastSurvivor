#include "BTDecorator_IsDead.h"
#include "LSMob.h"

UBTDecorator_IsDead::UBTDecorator_IsDead()
{
	NodeName = TEXT("IsDead");
}

bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ALSMob* Mob = Cast<ALSMob>(OwnerComp.GetAIOwner()->GetPawn());
	
	if(nullptr == Mob) return false;

	return Mob->IsDead();
}
