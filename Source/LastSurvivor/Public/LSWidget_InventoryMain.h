// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/GridPanel.h"
#include "Components/ScrollBox.h"
#include "LSWidget_InventoryMain.generated.h"

/**
 * 
 */


UCLASS()
class LASTSURVIVOR_API ULSWidget_InventoryMain : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateInventory();

private:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	void BuildTextureSlots();
	void BuildGroundItems();
	void BuildInventoryItems();
	
	TArray<UTexture2D*> Textures;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* ScrollBoxGroundItems;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* ScrollBoxWeapons;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* ScrollBoxHealthKits;

	UPROPERTY(meta=(BindWidget))
	UGridPanel* GridPanelTextures;
	
	UPROPERTY()
	ALSCharacter* CharacterRef;

	
};
