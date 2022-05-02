#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Widgets/Layout/SScaleBox.h"
#include "LSItem.generated.h"

UENUM(BlueprintType)
enum ItemType
{
	Weapon,
	HealthKit
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ItemType> ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	friend bool operator==(const FItemData& A, const FItemData& B)
	{
		return A.ID.IsEqual(B.ID);
	}

	friend bool operator<(const FItemData& A, const FItemData& B)
	{
		return A.Quantity < B.Quantity;
	}
};

UCLASS()
class LASTSURVIVOR_API ALSItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSItem();
	
	void RaiseQuantity();
	
	UFUNCTION(BlueprintCallable)
	FItemData GetData();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category=DataTable)
	UDataTable* DataTable;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ItemType> ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
};
