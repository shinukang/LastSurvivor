#include "LSWidget_HUD.h"

void ULSWidget_HUD::NativeConstruct()
{
	Super::NativeConstruct();
	CharacterRef = Cast<ALSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ULSWidget_HUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ProgressBarHP->PercentDelegate.BindUFunction(this, "GetHPRate");
	ProgressBarAmmo->PercentDelegate.BindUFunction(this, "GetAmmoRate");
	ProgressBarGrenade->PercentDelegate.BindUFunction(this, "GetGrenadeRate");
	ImageCurrentWeapon->BrushDelegate.BindUFunction(this, "GetCurrentWeaponImage");
	ImageCrosshair->ColorAndOpacityDelegate.BindUFunction(this, "SetCrosshairOpacity");
	ImageBloodFrame->ColorAndOpacityDelegate.BindUFunction(this, "SetBloodFrameOpacity");
}

float ULSWidget_HUD::GetHPRate()
{
	return CharacterRef->GetHealthPointRate();
}

float ULSWidget_HUD::GetAmmoRate()
{
	return CharacterRef->Inventory->GetCurrentWeapon()->GetAmmoRate();
}

float ULSWidget_HUD::GetGrenadeRate()
{
	return CharacterRef->GetGrenadeRemainingRate();
}

FSlateBrush ULSWidget_HUD::GetCurrentWeaponImage()
{
	FSlateBrush Icon = ImageCurrentWeapon->Brush;
	
	if(CharacterRef)
	{
		Icon = CharacterRef->Inventory->GetCurrentWeapon()->GetData().Icon;
		Icon.ImageSize = FVector2D(1200.0f, 60.0f);
	}
	return Icon; 
}

FLinearColor ULSWidget_HUD::SetCrosshairOpacity()
{
	const bool IsAiming = CharacterRef->IsAiming;
	const bool IsFiring = CharacterRef->IsFiring;

	FLinearColor CrosshairColor = ImageCrosshair->ColorAndOpacity;
	
	if(IsAiming || IsFiring)
	{
		CrosshairColor.A = 1.0f;
	}
	else
	{
		CrosshairColor.A = 0.0f;
	}
	return CrosshairColor;
}

FLinearColor ULSWidget_HUD::SetBloodFrameOpacity()
{
	FLinearColor BloodFrameColor = ImageBloodFrame->ColorAndOpacity;

	BloodFrameColor.A = CharacterRef->BloodFrameAlpha;

	return BloodFrameColor;
}





