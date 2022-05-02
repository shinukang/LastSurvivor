// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/BorderSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LSWidget_TextureSlot.generated.h"

/**
 * 
 */
UCLASS()
class LASTSURVIVOR_API ULSWidget_TextureSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;;
	virtual void NativeOnInitialized() override;
	void SetSlotData(UTexture2D* Texture, int32 Index);
	UFUNCTION(BlueprintCallable)
	FEventReply MouseDoubleClickCallback(FGeometry Geo, const FPointerEvent& MousePointer); 

private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextBlockSlot;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ImageSlot;
	
	UPROPERTY(meta=(BindWidget))
	UBorder* BorderSlot;

	UTexture2D* Texture;

	UPROPERTY()
	ALSCharacter* CharacterRef;

	int32 Index;
	
	FSlateBrush TextureBrush;

	
};
