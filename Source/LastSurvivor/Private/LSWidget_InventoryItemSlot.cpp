#include "LSWidget_InventoryItemSlot.h"

void ULSWidget_InventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULSWidget_InventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SlotBorder->OnMouseDoubleClickEvent.BindDynamic(this, &ULSWidget_InventoryItemSlot::MouseDoubleClickCallback);
}


FEventReply ULSWidget_InventoryItemSlot::MouseDoubleClickCallback(FGeometry Geometry, const FPointerEvent& MousePointer)
{
	FEventReply EventReply;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Success"));

	if(ItemData.ItemType == Weapon)
	{
		if(!(ItemData == CharacterRef->Inventory->GetCurrentWeapon()->GetData())) CharacterRef->TryEquip(ItemData);
	}
	else if(ItemData.ItemType == HealthKit)
	{
		CharacterRef->Recovery(ItemData);
	}

	WidgetRef->UpdateInventory();
	return EventReply;
}

void ULSWidget_InventoryItemSlot::SetSlotData(ALSCharacter* InputCharacterRef, ULSWidget_InventoryMain* InputWidgetRef, FItemData InputItemData)
{
	CharacterRef = InputCharacterRef;
	WidgetRef = InputWidgetRef;
	ItemData = InputItemData;

	QuantityTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData.Quantity)));
	NameTextBlock->SetText(FText::FromString(ItemData.Name.ToString()));
	IconImage->SetBrush(ItemData.Icon);
}

