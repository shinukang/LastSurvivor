// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "LSCharacter.h"
#include "LSMob.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("DetectCharacter");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr ==  Pawn) return;

	ALSMob* Mob = Cast<ALSMob>(Pawn);
	if(nullptr == Mob) return;

	const UWorld* World = Pawn->GetWorld() ;
	if(nullptr == World) return;

	const FVector Center = Pawn->GetActorLocation();
	
	const float DetectRadius = Mob->GetDetectRange();

	TArray<FOverlapResult> OverlapResults;
	const FCollisionQueryParams CollisionQueryParams(NAME_None, false, Pawn);

	const bool Result = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams);
	
	if(Result)
	{
		for(FOverlapResult OverlapResult : OverlapResults)
		{
			ALSCharacter* LSCharacter = Cast<ALSCharacter>(OverlapResult.GetActor());
			
			if(LSCharacter && LSCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ALSAIController::TargetKey, LSCharacter);
				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ALSAIController::TargetKey, nullptr);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ALSAIController::TargetKey, nullptr);
	}
}
