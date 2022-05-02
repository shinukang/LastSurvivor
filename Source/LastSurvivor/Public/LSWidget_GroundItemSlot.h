// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSCharacter.h"
#include "LSItem.h"
#include "LSWidget_InventoryMain.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LSWidget_GroundItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class LASTSURVIVOR_API ULSWidget_GroundItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuantityTextBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;

	UPROPERTY(meta=(BindWidget))
	UBorder* SlotBorder;

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void SetSlotData(ALSCharacter* CharacterRef, ULSWidget_InventoryMain* WidgetRef, ALSItem* ItemRef);

private:
	UFUNCTION(BlueprintCallable)
	FEventReply MouseButtonDownCallback(FGeometry Geometry, const FPointerEvent& MousePointer);

	ALSItem* ItemRef;
	ALSCharacter* CharacterRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ULSWidget_InventoryMain* WidgetRef;
	
};
