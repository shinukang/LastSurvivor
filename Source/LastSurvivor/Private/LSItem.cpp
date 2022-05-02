#include "LSItem.h"

ALSItem::ALSItem()
{

}

void ALSItem::BeginPlay()
{
	Super::BeginPlay();
}

void ALSItem::RaiseQuantity()
{
	Quantity += 1;
}

FItemData ALSItem::GetData()
{
	FItemData NewItemData;
	NewItemData.ID = ID;
	NewItemData.Icon = Icon;
	NewItemData.Name = Name;
	NewItemData.Quantity = Quantity;
	NewItemData.ItemType = ItemType;

	return NewItemData;
}





