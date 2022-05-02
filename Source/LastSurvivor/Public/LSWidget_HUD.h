// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "LSWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class LASTSURVIVOR_API ULSWidget_HUD : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	float GetHPRate();

	UFUNCTION(BlueprintCallable)
	float GetAmmoRate();

	UFUNCTION(BlueprintCallable)
	float GetGrenadeRate();

	UFUNCTION(BlueprintCallable)
	FSlateBrush GetCurrentWeaponImage();

	UFUNCTION(BlueprintCallable)
	FLinearColor SetCrosshairOpacity();

	UFUNCTION(BlueprintCallable)
	FLinearColor SetBloodFrameOpacity();


private:
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBarHP;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBarAmmo;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBarGrenade;

	UPROPERTY(meta=(BindWidget))
	UImage* ImageCurrentWeapon;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ImageCrosshair;

	UPROPERTY(meta=(BindWidget))
	UImage* ImageBloodFrame;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ALSCharacter* CharacterRef;
};
