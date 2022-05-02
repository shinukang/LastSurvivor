// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSMobSpawner.generated.h"

#define MAX_NUM_OF_MOB 100

UCLASS()
class LASTSURVIVOR_API ALSMobSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSMobSpawner();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UStaticMeshComponent* MeshComp;

	FTimerHandle SpawnTimer;
	
	int32 NumOfMobs;
	
	void SpawnMobs();
};
