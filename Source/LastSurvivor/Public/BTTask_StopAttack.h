#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopAttack.generated.h"

UCLASS()
class LASTSURVIVOR_API UBTTask_StopAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_StopAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
