#include "LSAIController.h"
#include "LSMob.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ALSAIController::TargetKey(TEXT("Target"));
const FName ALSAIController::HomePosKey(TEXT("HomePos"));
const FName ALSAIController::PatrolPosKey(TEXT("PatrolPos"));

ALSAIController::ALSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAsset(TEXT("BlackboardData'/Game/Mob/Mob_BB.Mob_BB'"));
	if(BBAsset.Succeeded())
	{
		BBMob = BBAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BHTAsset(TEXT("BehaviorTree'/Game/Mob/Mob_BHT.Mob_BHT'"));
	if(BHTAsset.Succeeded())
	{
		BHTMob = BHTAsset.Object;
	}
}



void ALSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto Mob = Cast<ALSMob>(InPawn);
	
	if(nullptr != Mob && UseBlackboard(BBMob, Blackboard))
	{
		RunBehaviorTree(BHTMob);
		Blackboard->SetValueAsFloat(FName("DetectRange"), Mob->GetDetectRange());
		Blackboard->SetValueAsFloat(FName("IsDead"), Mob->IsDead());
		Blackboard->SetValueAsFloat(FName("IsHit"), Mob->IsHit());
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Possessed"));
	}
}

