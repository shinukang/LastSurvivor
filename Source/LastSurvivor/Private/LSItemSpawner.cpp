#include "LSItemSpawner.h"
#include "LSHealthKit.h"
#include "LSWeapon.h"
#include "Kismet/KismetMathLibrary.h"

#define MAX_WEAPON 5
#define MAX_HEALTH_KIT 2

ULSItemSpawner::ULSItemSpawner()
{
	for(int i = 1; i <= MAX_WEAPON; i++)
	{
		WeaponIDList.Add(FName(FString("W00") + FString::FromInt(i)));
	}

	for(int j = 1; j <= MAX_HEALTH_KIT; j++)
	{
		HealthKitIDList.Add(FName(FString("H00") + FString::FromInt(j)));
	}
}

void ULSItemSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ULSItemSpawner::SpawnAllItems(FVector OriginLocation)
{
	for(const FName WeaponID : WeaponIDList)
	{
		const float RandomSeed = 100.0f;
		
		const float RandomX = UKismetMathLibrary::RandomFloatInRange(RandomSeed * -3, RandomSeed * 3); 
		const float RandomY = UKismetMathLibrary::RandomFloatInRange(RandomSeed * -3, RandomSeed * 3);

		FVector SpawnLocation = FVector(OriginLocation.X + RandomX, OriginLocation.Y + RandomY, OriginLocation.Z);
		FRotator SpawnRotation = FRotator(90.0f, 0.0f, 0.0f);	
		ALSWeapon* NewWeapon = GetWorld()->SpawnActor<ALSWeapon>(SpawnLocation, SpawnRotation);
		NewWeapon->Initialize(WeaponID);
		NewWeapon->SetLifeSpan(15.0f);
	}

	for(FName const HealthKitID : HealthKitIDList)
	{
		const float RandomSeed = 100.0f;
		
		const float RandomX = UKismetMathLibrary::RandomFloatInRange(RandomSeed * -3, RandomSeed * 3); 
		const float RandomY = UKismetMathLibrary::RandomFloatInRange(RandomSeed * -3, RandomSeed * 3);

		FVector SpawnLocation = FVector(OriginLocation.X + RandomX, OriginLocation.Y + RandomY, OriginLocation.Z);
		FRotator SpawnRotation = FRotator(90.0f, 0.0f, 0.0f);	
		ALSHealthKit* NewHealthKit = GetWorld()->SpawnActor<ALSHealthKit>(SpawnLocation, SpawnRotation);
		NewHealthKit->Initialize(HealthKitID);
		NewHealthKit->SetLifeSpan(15.0f);
	}
}

void ULSItemSpawner::SpawnItems(FVector OriginLocation, int32 NumOfItems)
{
	for(int i = 0; i < NumOfItems; i++)
	{
		const int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, MAX_WEAPON + MAX_HEALTH_KIT - 1);

		FName ItemID;

		const float RandomSeed = 100.0f;
		const float RandomX = UKismetMathLibrary::RandomFloatInRange(RandomSeed * -3, RandomSeed * 3); 
		const float RandomY = UKismetMathLibrary::RandomFloatInRange(RandomSeed * -3, RandomSeed * 3);

		FVector SpawnLocation = FVector(OriginLocation.X + RandomX, OriginLocation.Y + RandomY, OriginLocation.Z);
		FRotator SpawnRotation = FRotator(90.0f, 0.0f, 0.0f);	
		
		if(RandomIndex >= 0 && RandomIndex <= MAX_WEAPON - 1)
		{
			ItemID = WeaponIDList[RandomIndex];
			ALSWeapon* NewWeapon = GetWorld()->SpawnActor<ALSWeapon>(SpawnLocation, SpawnRotation);
			NewWeapon->Initialize(ItemID);
			NewWeapon->SetLifeSpan(15.0f);
		}
		else if(RandomIndex >= MAX_WEAPON && RandomIndex <= MAX_WEAPON + MAX_HEALTH_KIT - 1)
		{
			ItemID = HealthKitIDList[RandomIndex - 5];
			ALSHealthKit* NewHealthKit = GetWorld()->SpawnActor<ALSHealthKit>(SpawnLocation, SpawnRotation);
			NewHealthKit->Initialize(ItemID);
			NewHealthKit->SetLifeSpan(15.0f);
		}
	}
}


