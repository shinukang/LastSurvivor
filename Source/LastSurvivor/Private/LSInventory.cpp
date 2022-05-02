#include "LSInventory.h"
#include "LSGrenade.h"
#include "LSHealthKit.h"
#include "LSWeapon.h"
#include "Blueprint/UserWidget.h"

ULSInventory::ULSInventory()
{
	
}


void ULSInventory::AddToInventory(ALSItem* Item)
{
	const FItemData NewItemData = Item->GetData();
	
	for(FItemData &ItemData : InventoryItems)
	{
		if(ItemData.ID == NewItemData.ID)
		{
			ItemData.Quantity += 1;
			GroundItems.Remove(Item);
			InventoryItems.Sort();
			Algo::Reverse(InventoryItems);
			if(Item->GetData() == CurrentWeapon->GetData())
			{
				CurrentWeapon->Initialize(ItemData);
			}
			if(Item != CurrentWeapon) Item->Destroy();
			return;
		}
	}
	InventoryItems.Add(NewItemData);
	GroundItems.Remove(Item);
	if(Item != CurrentWeapon) Item->Destroy();
}

void ULSInventory::AddToGround(ALSItem* Item)
{
	GroundItems.Add(Item);
}

void ULSInventory::ClearItemsOnGround()
{
	GroundItems.Empty();
}

void ULSInventory::SetCurrentWeapon(ALSWeapon* Weapon)
{
	CurrentWeapon = Weapon;
}

ALSWeapon* ULSInventory::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void ULSInventory::UseItem(int32 Index)
{
	if(InventoryItems[Index].Quantity <= 1)
	{
		InventoryItems.RemoveAt(Index);
	}
	else
	{
		InventoryItems[Index].Quantity -= 1;
	}
}









