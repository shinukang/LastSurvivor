#include "LSPlayerController.h"
#include "LSWidget_HUD.h"
#include "LSWidget_InventoryMain.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Framework/Application/NavigationConfig.h"

void ALSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(IsLocalController())
	{
		TSharedRef<FNavigationConfig> Navigation = MakeShared<FNavigationConfig>();
		Navigation->bKeyNavigation = false;
		Navigation->bTabNavigation = false;
		Navigation->bAnalogNavigation = false;
		FSlateApplication::Get().SetNavigationConfig(Navigation);
	}
}

void ALSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LSLOG_S(Warning);
}

void ALSPlayerController::OnPossess(APawn* InPawn)
{
	LSLOG_S(Warning);
	Super::OnPossess(InPawn);

	const FSoftClassPath MainWidgetClassRef(TEXT("/Game/HUD/HUD.HUD_C"));

	if(UClass* WidgetClass = MainWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		MainWidget = CreateWidget<ULSWidget_HUD>(this, WidgetClass);
		MainWidget->AddToViewport();
	}
}

void ALSPlayerController::SetInputUI()
{
	FInputModeGameAndUI Mode;
	Mode.SetWidgetToFocus(InventoryWidget->GetCachedWidget());
	Mode.SetHideCursorDuringCapture(false);
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	bShowMouseCursor = true;
	SetInputMode(Mode);
}

void ALSPlayerController::SetInputGame()
{
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void ALSPlayerController::CreateInventoryWidget()
{
	if(IsValid(InventoryWidget))
	{
		const FSoftClassPath InventoryWidgetClassRef(TEXT("/Game/HUD/Inventory.Inventory_C"));

		if(UClass* WidgetClass =  InventoryWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			InventoryWidget = CreateWidget<ULSWidget_InventoryMain>(this, WidgetClass);
		}
	}
	InventoryWidget->AddToViewport();
	SetInputUI();
}

void ALSPlayerController::DestroyInventoryWidget()
{
	if(IsValid(InventoryWidget))
	{
		InventoryWidget->RemoveFromViewport();
		SetInputGame();
	}
}




