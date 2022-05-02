#pragma once

#include "CoreMinimal.h"
#include "LSItem.h"
#include "LSWeapon.h"
#include "Components/ActorComponent.h"
#include "LSInventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LASTSURVIVOR_API ULSInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ALSItem*> GroundItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemData> InventoryItems;
	
	ULSInventory();

	void AddToInventory(ALSItem* Item);
	
	void AddToGround(ALSItem* Item);

	void ClearItemsOnGround();

	ALSWeapon* GetCurrentWeapon();
	
	void SetCurrentWeapon(ALSWeapon* Weapon);
	
	void UseItem(int32 Index);

private:
	UPROPERTY()
	ALSWeapon* CurrentWeapon;
	
};
