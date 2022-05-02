// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastSurvivor.h"
#include "GameFramework/PlayerController.h"
#include "LSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LASTSURVIVOR_API ALSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	UPROPERTY()
	UUserWidget* MainWidget;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	void SetInputUI();
	void SetInputGame();

	void CreateInventoryWidget();
	void DestroyInventoryWidget();

	

	
	
};

