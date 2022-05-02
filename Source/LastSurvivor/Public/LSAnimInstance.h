#pragma once

#include "LastSurvivor.h"
#include "LSWeapon.h"
#include "Animation/AnimInstance.h"
#include "LSAnimInstance.generated.h"


UENUM()
enum EFootStepState
{
	FSS_Left,
	FSS_Right,
	FSS_Land
};

USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* Fire;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Reload;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Equip;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ThrowFar;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ThrowClose;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Death;
};

DECLARE_DELEGATE(FOnFireDelegate);
DECLARE_DELEGATE(FOnThrowDelegate);
DECLARE_DELEGATE(FOnEquipDelegate);
DECLARE_DELEGATE_OneParam(FOnFootStepDelegate, EFootStepState);

UCLASS()
class LASTSURVIVOR_API ULSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULSAnimInstance();
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	FOnFireDelegate OnFire;
	FOnThrowDelegate OnThrow;
	FOnEquipDelegate OnEquip;
	FOnFootStepDelegate OnFootStep;
	
	void PlayReloadMontage();
	void PlayFireMontage();
	void PlayThrowMontage();
	void PlayEquipMontage();
	
	bool IsReloadMontage(const UAnimMontage* Montage);
	bool IsEquipMontage(const UAnimMontage* Montage);
	bool IsThrowMontage(const UAnimMontage* Montage);
	
private:
	UPROPERTY()
	UDataTable* MontageDataTable;

	UPROPERTY()
	FMontageData PistolMontages;

	UPROPERTY()
	FMontageData RifleMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	float Controller_Pitch;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=State, meta=(AllowPrivateAccess=true))
	bool IsJumping;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=State, meta=(AllowPrivateAccess=true))
	bool IsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=State, meta=(AllowPrivateAccess=true))
	bool IsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=State, meta=(AllowPrivateAccess=true))
	bool IsThrowing;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=State, meta=(AllowPrivateAccess=true))
	TEnumAsByte<EWeaponType> CurrentWeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=State, meta=(AllowPrivateAccess=true))
	FVector CrossHairDirection;
	
	UFUNCTION()
	void AnimNotify_Fire();

	UFUNCTION()
	void AnimNotify_Throw();

	UFUNCTION()
	void AnimNotify_Equip();
	
	UFUNCTION()
	void AnimNotify_FootStepLeft();

	UFUNCTION()
	void AnimNotify_FootStepRight();

	UFUNCTION()
	void AnimNotify_FootStepLand();
};


