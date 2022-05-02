#include "LSAnimInstance.h"
#include "LSCharacter.h"
#include "Engine.h"
#include "Math/Vector.h"
#include "LSWeapon.h"
#include "Windows/AllowWindowsPlatformTypes.h"

ULSAnimInstance::ULSAnimInstance()
{
	IsJumping = false;
	IsAiming = false;
	IsFiring = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Montage(TEXT("DataTable'/Game/Character/DataTables/MontageDataTable.MontageDataTable'"));
	if(DT_Montage.Succeeded())
	{
		MontageDataTable = DT_Montage.Object;

		if(MontageDataTable != nullptr)
		{
			PistolMontages = *MontageDataTable->FindRow<FMontageData>(FName(TEXT("Pistol")), FString("Error"));
			RifleMontages = *MontageDataTable->FindRow<FMontageData>(FName(TEXT("Rifle")), FString("Error"));
		}
	}
}

void ULSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	const auto Pawn = TryGetPawnOwner();

	if(IsValid(Pawn))
	{
		Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
		Speed = FMath::Sqrt(FMath::Pow(Pawn->GetVelocity().X, 2) + FMath::Pow(Pawn->GetVelocity().Y,2));
		Controller_Pitch = Pawn->GetControlRotation().Pitch;
	
		const auto Character = Cast<ACharacter>(Pawn);
		if(IsValid(Character))
		{
			IsJumping = Character->GetMovementComponent()->IsFalling();

			const auto LSCharacter = Cast<ALSCharacter>(Character);
			if(IsValid(LSCharacter))
			{
				CrossHairDirection = LSCharacter->GetFireDirection();
				IsAiming = LSCharacter->IsAiming;
				IsFiring = LSCharacter->IsFiring;
				IsThrowing = LSCharacter->IsThrowing;

				const auto LSWeapon = LSCharacter->Inventory->GetCurrentWeapon();
				if(IsValid(LSWeapon))
				{
					CurrentWeaponType = LSWeapon->GetWeaponType();
				}
			}
		}
	}
}

void ULSAnimInstance::AnimNotify_Fire()
{
	OnFire.Execute();
}

void ULSAnimInstance::AnimNotify_Throw()
{
	OnThrow.Execute();
}

void ULSAnimInstance::AnimNotify_Equip()
{
	OnEquip.Execute();
}

void ULSAnimInstance::AnimNotify_FootStepLeft()
{
	OnFootStep.Execute(FSS_Left);
}

void ULSAnimInstance::AnimNotify_FootStepRight()
{
	OnFootStep.Execute(FSS_Right);
}

void ULSAnimInstance::AnimNotify_FootStepLand()
{
	OnFootStep.Execute(FSS_Land);
}

void ULSAnimInstance::PlayFireMontage()
{
	if(CurrentWeaponType == Pistol)
	{
		Montage_Play(PistolMontages.Fire);
	}
	else
	{
		Montage_Play(RifleMontages.Fire);
	}
}

void ULSAnimInstance::PlayReloadMontage()
{
	if(CurrentWeaponType == Pistol)
	{
		Montage_Play(PistolMontages.Reload);
	}
	else
	{
		Montage_Play(RifleMontages.Reload);
	}
}

void ULSAnimInstance::PlayThrowMontage()
{
	if(CurrentWeaponType == Pistol)
	{
		if(CrossHairDirection.Z > 0)
		{
			Montage_Play(PistolMontages.ThrowFar);
		}
		else
		{
			Montage_Play(PistolMontages.ThrowClose);
		}
	}
	else
	{
		if(CrossHairDirection.Z > 0)
		{
			Montage_Play(RifleMontages.ThrowFar);
		}
		else
		{
			Montage_Play(RifleMontages.ThrowClose);
		}
	}
}

void ULSAnimInstance::PlayEquipMontage()
{
	if(CurrentWeaponType == Pistol)
	{
		Montage_Play(PistolMontages.Equip);	
	}
	else
	{
		Montage_Play(RifleMontages.Equip);
	}
}

bool ULSAnimInstance::IsEquipMontage(const UAnimMontage* Montage)
{
	if(Montage == RifleMontages.Equip || Montage == PistolMontages.Equip)
	{
		return true;
	}
	return false;
}

bool ULSAnimInstance::IsReloadMontage(const UAnimMontage* Montage)
{
	if(Montage == RifleMontages.Reload || Montage == PistolMontages.Reload)
	{
		return true;
	}
	return false;
}

bool ULSAnimInstance::IsThrowMontage(const UAnimMontage* Montage)
{
	if(Montage == RifleMontages.ThrowFar || Montage == RifleMontages.ThrowClose || Montage == PistolMontages.ThrowFar || Montage == PistolMontages.ThrowClose)
	{
		return true;
	}
	return false;
}



