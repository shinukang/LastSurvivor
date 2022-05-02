#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSItemSpawner.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTSURVIVOR_API ULSItemSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULSItemSpawner();
	
protected:
	virtual void BeginPlay() override;

public:
	void SpawnAllItems(FVector OriginLocation);
	void SpawnItems(FVector OriginLocation, int32 NumOfItems);


private:
	TArray<FName> WeaponIDList;
	TArray<FName> HealthKitIDList;
};
