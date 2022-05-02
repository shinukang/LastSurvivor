#include "LSHealthKit.h"
#include "LastSurvivor.h"

ALSHealthKit::ALSHealthKit()
{
	ItemType = HealthKit;
 	HealthKitMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthKitMesh"));
	HealthKitMeshComp->SetCollisionProfileName(FName("HealthKit"));
	RootComponent = HealthKitMeshComp;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_HealthKit(TEXT("DataTable'/Game/HealthKit/HealthKitDataTable.HealthKitDataTable'"));
	if(DT_HealthKit.Succeeded())
	{
		DataTable = DT_HealthKit.Object;
	}
}

void ALSHealthKit::Initialize(FName HealthKitID)
{
	if(DataTable)
	{
		const FHealthKitData* HealthKitData = DataTable->FindRow<FHealthKitData>(HealthKitID, FString("Warn"));
		if(HealthKitData)
		{
			HealthKitMeshComp->SetStaticMesh(HealthKitData->HealthKitMesh);

			ID = HealthKitData->ID;
			Name = HealthKitData->Name;
			Icon = HealthKitData->Icon;
			Quantity = HealthKitData->Quantity;
			RecoveryRate = HealthKitData->RecoveryRate;
		}
		else
		{
			LSLOG_S(Error);
		}
	}
}


