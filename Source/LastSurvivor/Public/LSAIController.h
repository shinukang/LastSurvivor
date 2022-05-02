#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LSAIController.generated.h"

UCLASS()
class LASTSURVIVOR_API ALSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALSAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName TargetKey;
	static const FName PatrolPosKey;
	static const FName HomePosKey;
	
private:
	UPROPERTY()
	UBehaviorTree* BHTMob;

	UPROPERTY()
	UBlackboardData* BBMob;
	
};
