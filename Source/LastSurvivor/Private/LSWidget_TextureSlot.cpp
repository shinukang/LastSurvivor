#include "LSWidget_TextureSlot.h"
#include "LSCharacter.h"
#include "Components/Border.h"

void ULSWidget_TextureSlot::NativeConstruct()
{
	CharacterRef = Cast<ALSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}

void ULSWidget_TextureSlot::NativeOnInitialized()
{
	BorderSlot->OnMouseDoubleClickEvent.BindDynamic(this, &ULSWidget_TextureSlot::MouseDoubleClickCallback);
}

void ULSWidget_TextureSlot::SetSlotData(UTexture2D* InputTexture, int32 InputIndex)
{
	if(InputTexture)
	{
		ImageSlot->SetBrushFromTexture(InputTexture);

		TextBlockSlot->SetText(FText::FromString(InputTexture->GetName()));
	
		Texture = InputTexture;

		Index = InputIndex;
	}
	
}


FEventReply ULSWidget_TextureSlot::MouseDoubleClickCallback(FGeometry Geo, const FPointerEvent& MousePointer)
{
	FEventReply EventReply;
	
	CharacterRef->SetMaterialTexture(Texture);
	
	return EventReply;
}




