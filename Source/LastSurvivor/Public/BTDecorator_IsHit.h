#pragma once

#include "LastSurvivor.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsHit.generated.h"

UCLASS()
class LASTSURVIVOR_API UBTDecorator_IsHit : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsHit();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
