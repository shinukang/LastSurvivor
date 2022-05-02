#pragma once

#include "CoreMinimal.h"
#include "LSItem.h"
#include "Engine/DataTable.h"
#include "LSHealthKit.generated.h"

USTRUCT(BlueprintType)
struct FHealthKitData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* HealthKitMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoveryRate;
};

UCLASS()
class LASTSURVIVOR_API ALSHealthKit : public ALSItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float RecoveryRate;

	ALSHealthKit();

	void Initialize(FName HealthKitID);
	
private:
	UPROPERTY()
	UStaticMeshComponent* HealthKitMeshComp;
};
