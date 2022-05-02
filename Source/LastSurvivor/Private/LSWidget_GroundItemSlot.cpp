#include "LSWidget_GroundItemSlot.h"

void ULSWidget_GroundItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULSWidget_GroundItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SlotBorder->OnMouseButtonDownEvent.BindDynamic(this, &ULSWidget_GroundItemSlot::MouseButtonDownCallback);
}

FEventReply ULSWidget_GroundItemSlot::MouseButtonDownCallback(FGeometry Geometry, const FPointerEvent& MousePointer)
{
	FEventReply EventReply;
	
	CharacterRef->Inventory->AddToInventory(ItemRef);
	RemoveFromParent();
	WidgetRef->UpdateInventory();
	return EventReply;
}

void ULSWidget_GroundItemSlot::SetSlotData(ALSCharacter* InputCharacterRef, ULSWidget_InventoryMain* InputWidgetRef, ALSItem* InputItemRef)
{
	CharacterRef = InputCharacterRef;
	WidgetRef = InputWidgetRef;
	ItemRef = InputItemRef;
	
	const FItemData ItemData = ItemRef->GetData();
	
	QuantityTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData.Quantity)));
	NameTextBlock->SetText(FText::FromString(ItemData.Name.ToString()));
	IconImage->SetBrush(ItemData.Icon);
}


