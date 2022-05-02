#include "BTDecorator_IsHit.h"
#include "LSMob.h"

UBTDecorator_IsHit::UBTDecorator_IsHit()
{
	NodeName = TEXT("IsHit");
}

bool UBTDecorator_IsHit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	
	ALSMob* Mob = Cast<ALSMob>(OwnerComp.GetAIOwner()->GetPawn());
	
	if(nullptr == Mob) return false;

	return Mob->IsHit();
}