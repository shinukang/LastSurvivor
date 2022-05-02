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
#include "LSWidget_InventoryItemSlot.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FUpdateWidget);

UCLASS()
class LASTSURVIVOR_API ULSWidget_InventoryItemSlot : public UUserWidget
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

	FUpdateWidget UpdateWidget;

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void SetSlotData(ALSCharacter* CharacterRef, ULSWidget_InventoryMain* WidgetRef, FItemData MewItemData);

private:
	
	UFUNCTION(BlueprintCallable)
	FEventReply MouseDoubleClickCallback(FGeometry Geo, const FPointerEvent& MousePointer); 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FItemData ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ALSCharacter* CharacterRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ULSWidget_InventoryMain* WidgetRef;
};
