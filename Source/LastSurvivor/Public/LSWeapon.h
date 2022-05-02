// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LastSurvivor.h"
#include "LSItem.h"
#include "MatineeCameraShake.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "LSWeapon.generated.h"

UENUM(BlueprintType)
enum EWeaponType
{
	Pistol,
	Rifle,
	Shotgun,
	MachineGun
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EWeaponType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* MagMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* ReloadSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* OutOfAmmoSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* FireEffect;
};

UCLASS()
class LASTSURVIVOR_API ALSWeapon : public ALSItem
{
	GENERATED_BODY()
	
public:	
	ALSWeapon();
	void Initialize(FName WeaponName);
	void Initialize(FItemData WeaponData);
	
	USkeletalMeshComponent* GetMesh();
	TEnumAsByte<EWeaponType> GetWeaponType();
	FName GetWeaponName();
	float GetAmmoRate();

	void Fire(FVector GoalDirection);
	void Reload();
	void PlayReloadSound();

	int32 GetMaxAmmo();
	int32 GetCurrentAmmo();
	float GetRateOfFire();

	FTimerHandle ReFireCheckTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float Damage;

private:
	UPROPERTY(EditAnywhere, Category=Mesh)
	USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(EditAnywhere, Category=Mesh)
	USkeletalMeshComponent* MagMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TEnumAsByte<EWeaponType> Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	USkeletalMesh* MagMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	int32 CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FName AmmoType; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	USoundCue* ReloadSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	USoundCue* OutOfAmmoSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UParticleSystem* FireEffect;
	
	UPROPERTY()
	TSubclassOf<UMatineeCameraShake> RecoilCameraShake;
};