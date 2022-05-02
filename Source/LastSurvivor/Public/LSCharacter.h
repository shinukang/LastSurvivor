#pragma once

#include "LastSurvivor.h"
#include "GameFramework/Character.h"
#include "SkeletalMeshMerge.h"
#include "LSAnimInstance.h"
#include "LSInventory.h"
#include "LSWeapon.h"
#include "MatineeCameraShake.h"
#include "Components/TimelineComponent.h"
#include "LSCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FEquipWeaponDelegate, int32);

USTRUCT(BlueprintType)
struct FFootStepFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* LeftFootStepSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* RightFootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* LandFootStepSound;
};

UCLASS()
class LASTSURVIVOR_API ALSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Inventory, meta=(AllowPrivateAccess=true))
	ULSInventory* Inventory;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, meta=(AllowPrivateAccess=true));
	bool IsInWidget;
		
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, meta=(AllowPrivateAccess=true));
	bool IsFiring;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, meta=(AllowPrivateAccess=true));
	bool IsThrowing;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, meta=(AllowPrivateAccess=true));
	bool IsReloading;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, meta=(AllowPrivateAccess=true));
	bool IsAiming;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, meta=(AllowPrivateAccess=true));
	bool IsEquipping;

	float BloodFrameAlpha;
	
	ALSCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	
	void TryEquip(FItemData WeaponData);
	void Recovery(FItemData HealthKitData);

	void SetMaterialTexture(UTexture2D* Texture);
	
	float GetHealthPointRate();
	float GetGrenadeRemainingRate();
	FVector GetCrossHairImpactPoint();
	FVector GetFireDirection();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* Camera;
	
	FVector2D CrossHairLocation;
	
	FTimeline AimingTimeline;
	FTimeline BloodFrameTimeline;

	UPROPERTY()
	UMaterialInstanceDynamic* MatInstance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Timeline, meta=(AllowPrivateAccess=true))
	UCurveFloat* AimingCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Timeline, meta=(AllowPrivateAccess=true))
	UCurveFloat* BloodFrameCurve;

	TSubclassOf<UMatineeCameraShake> HitCameraShake; 

	FTimerHandle WeaponTimer;
	FTimerHandle GrenadeTimer;
	float GrenadeDelay;
	
	UPROPERTY()
	ULSAnimInstance* LSAnim;

	UPROPERTY()
	UDataTable* FootStepDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Spec, meta=(AllowPrivateAccess=true));
	float MaxHealthPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Spec, meta=(AllowPrivateAccess=true));
	float CurrentHealthPoint;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void SetControlMode();
	
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
    void Turn(float NewAxisValue);
	
	void OpenInventory();
	void CloseInventory();
	void FindItemsOnGround();
	
	void TryFire();
	void OnFire();
	
	UFUNCTION()
	void Fire();
    void StopFire();

	void TryThrow();
	UFUNCTION()
	void Throw();

	void Equip(ALSWeapon* Weapon);
	
	UFUNCTION()
	void Equip();
	
	void StartRun();
	void StopRun();

	void TryReload();
	void Reload();
	
	void StartAim();
	void StopAim();
	
	bool CanFire();
	bool CanReload();
	bool CanThrow();
	bool CanEquip();

	UFUNCTION()
	void AimingProgress(float Value);

	UFUNCTION()
	void BloodFrameProgress(float Value);

	UFUNCTION()
	void PlayFootStep(TEnumAsByte<EFootStepState> State);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};

