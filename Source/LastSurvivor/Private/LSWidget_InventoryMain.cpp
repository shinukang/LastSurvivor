#include "LSWidget_InventoryMain.h"
#include "AIController.h"
#include "LSWidget_GroundItemSlot.h"
#include "LSWidget_InventoryItemSlot.h"
#include "LSWidget_TextureSlot.h"

void ULSWidget_InventoryMain::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterRef = Cast<ALSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TArray<FString> ArrayOfName = { FString("Desert"), FString("Digital"), FString("Jungle"), FString("Specops"), FString("Swat"), FString("Winter")};
	
	for(FString Name : ArrayOfName)
	{
		FString Path = TEXT("Texture2D'/Game/Character/textures/" + Name + "." + Name + "'");
		
		auto NewTexture = LoadObject<UTexture2D>(nullptr, *Path);
		
		Textures.Add(NewTexture);
	}
	UpdateInventory();
	BuildTextureSlots();
}

void ULSWidget_InventoryMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULSWidget_InventoryMain::BuildTextureSlots()
{
	TQueue<ULSWidget_TextureSlot*> ArrayOfSlots;
	
	for(int i = 0; i < 6; i++)
	{
		UTexture2D* Texture = Textures[i];
		
		FSoftClassPath TextureSlotWidgetClassRef(TEXT("/Game/HUD/TextureSlot.TextureSlot_C"));
		
		if(UClass* WidgetClass =TextureSlotWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			auto const TextureSlot = CreateWidget<ULSWidget_TextureSlot>(this, WidgetClass);

			if(TextureSlot)
			{
				TextureSlot->SetSlotData(Texture, i);
				
				ArrayOfSlots.Enqueue(TextureSlot);
				
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, Texture->GetName());
			}
		}
	}
	for(int j = 0; j < 2; j++)
	{
		for(int k = 0; k < 3; k++)
		{
			ULSWidget_TextureSlot* NewSlot;
			if(ArrayOfSlots.Dequeue(NewSlot))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Dequeue"));
				GridPanelTextures->AddChildToGrid(NewSlot, j, k);
			}
			else
			{
				break;
			}
		}
	}
}

void ULSWidget_InventoryMain::BuildGroundItems()
{
	ScrollBoxGroundItems->ClearChildren();
										
	for(ALSItem* Item : CharacterRef->Inventory->GroundItems)
	{
		FSoftClassPath GroundItemSlotWidgetClassRef(TEXT("/Game/HUD/GroundItemSlot.GroundItemSlot_C"));
		
		if(UClass* WidgetClass = GroundItemSlotWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			auto const GroundItemSlot = CreateWidget<ULSWidget_GroundItemSlot>(this, WidgetClass);
			if(GroundItemSlot == nullptr)
			{
			}
			else
			{
				GroundItemSlot->SetSlotData(CharacterRef, this, Item);
				ScrollBoxGroundItems->AddChild(GroundItemSlot);
			}
		}
	}
}

void ULSWidget_InventoryMain::BuildInventoryItems()
{
	ScrollBoxWeapons->ClearChildren();
	ScrollBoxHealthKits->ClearChildren();

	for(const FItemData ItemData : CharacterRef->Inventory->InventoryItems)
	{
		FSoftClassPath InventoryItemSlotWidgetClassRef(TEXT("/Game/HUD/InventoryItemSlot.InventoryItemSlot_C"));

		if(UClass* WidgetClass = InventoryItemSlotWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			auto const InventoryItemSlot = CreateWidget<ULSWidget_InventoryItemSlot>(this, WidgetClass);
			InventoryItemSlot->SetSlotData(CharacterRef, this, ItemData);

			switch(ItemData.ItemType)
			{
			case Weapon:
				ScrollBoxWeapons->AddChild(InventoryItemSlot);
				break;
			case HealthKit:
				ScrollBoxHealthKits->AddChild(InventoryItemSlot);
				break;

				default:
					break;
			}
		}
	}
}

void ULSWidget_InventoryMain::UpdateInventory()
{
	BuildGroundItems();
	BuildInventoryItems();
}


