#include "LSWeapon.h"
#include "Engine.h"
#include "LSAmmo.h"
#include "LSCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ALSWeapon::ALSWeapon()
{
	ItemType = Weapon;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	MagMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagMesh"));

	WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
	WeaponMeshComp->SetCollisionProfileName(FName("Weapon"));
	WeaponMeshComp->SetEnableGravity(true);
	WeaponMeshComp->SetGenerateOverlapEvents(true);
	
	MagMeshComp->SetupAttachment(WeaponMeshComp);
	
	RootComponent = WeaponMeshComp;

	static ConstructorHelpers::FClassFinder<UMatineeCameraShake> CS_Recoil(TEXT("/Game/HUD/CamShake_Recoil.CamShake_Recoil_C"));
	if(CS_Recoil.Succeeded())
	{
		RecoilCameraShake = CS_Recoil.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Weapon(TEXT("DataTable'/Game/Weapons/WeaponDataTable.WeaponDataTable'"));
	if(DT_Weapon.Succeeded())
	{
		DataTable = DT_Weapon.Object;
	}
}

void ALSWeapon::Initialize(FName WeaponName)
{
	if(IsValid(DataTable))
	{
		const FWeaponData* Data = DataTable->FindRow<FWeaponData>(WeaponName, FString("Warn"));
		if(Data)
		{
			WeaponMeshComp->SetSkeletalMesh(Data->WeaponMesh);
			MagMeshComp->SetSkeletalMesh(Data->MagMesh);
			MagMeshComp->AttachToComponent(WeaponMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Mag"))); 

			ID = Data->ID;
			Name = Data->Name;
			Icon = Data->Icon;
			Type = Data->Type;
			Quantity = Data->Quantity;

			FireSound = Data->FireSound;
			OutOfAmmoSound = Data->OutOfAmmoSound;
			ReloadSound = Data->ReloadSound; 
			FireEffect = Data->FireEffect;

			MaxAmmo = Data->MaxAmmo;
			CurrentAmmo = MaxAmmo;
			RateOfFire = Data->RateOfFire;
			AmmoType = Data->AmmoType;
			BaseDamage = Data->BaseDamage;
			Damage = BaseDamage;
		}
		else
		{
			LSLOG_S(Error);
		}
	}
}

void ALSWeapon::Initialize(FItemData WeaponData)
{
	Initialize(WeaponData.ID);
	Quantity = WeaponData.Quantity;
	Damage = BaseDamage * FMath::Pow(1.05f, Quantity - 1);
}

void ALSWeapon::Fire(FVector GoalDirection)
{
	if(CurrentAmmo > 0)
	{
		FVector StartLocation = WeaponMeshComp->GetSocketLocation(FName("Barrel_Squared")); 

		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAttached(FireEffect, WeaponMeshComp, FName("Barrel_Squared")); 

		if(Type == Shotgun)
		{
			for(int i = 0; i < 12; i++)
			{
				const float SpreadRatio = 0.1f;
				const float SpreadX = UKismetMathLibrary::RandomFloatInRange(-SpreadRatio, SpreadRatio); 
				const float SpreadY = UKismetMathLibrary::RandomFloatInRange(-SpreadRatio, SpreadRatio);
				const float SpreadZ = UKismetMathLibrary::RandomFloatInRange(-SpreadRatio, SpreadRatio);
				
				ALSAmmo* Ammo = GetWorld()->SpawnActor<ALSAmmo>(StartLocation, GoalDirection.Rotation());
				Ammo->Initialize(AmmoType);
				if(Ammo)
				{
					FVector SpreadDirection = FVector(GoalDirection.X + SpreadX, GoalDirection.Y + SpreadY, GoalDirection.Z + SpreadZ);
					Ammo->FireInDirection(SpreadDirection, Damage / 12.0f);
				}
			}
		}
		else
		{
			ALSAmmo* Ammo = GetWorld()->SpawnActor<ALSAmmo>(StartLocation, GoalDirection.Rotation());
			Ammo->Initialize(AmmoType);
			if(IsValid(Ammo))
			{
				Ammo->FireInDirection(GoalDirection, Damage);
			}
		}
		CurrentAmmo--;
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(RecoilCameraShake);
	}
	else 
	{
		UGameplayStatics::PlaySoundAtLocation(this, OutOfAmmoSound, GetActorLocation());
	}
}

void ALSWeapon::PlayReloadSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}

void ALSWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;
}

int32 ALSWeapon::GetMaxAmmo()
{
	return MaxAmmo;
}

int32 ALSWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}

float ALSWeapon::GetRateOfFire()
{
	return RateOfFire;
}

USkeletalMeshComponent* ALSWeapon::GetMesh()
{
	return WeaponMeshComp;
}

TEnumAsByte<EWeaponType> ALSWeapon::GetWeaponType()
{
	return Type;
}

FName ALSWeapon::GetWeaponName()
{
	return Name;
}

float ALSWeapon::GetAmmoRate()
{
	return static_cast<float>(CurrentAmmo) / static_cast<float>(MaxAmmo);
}




	