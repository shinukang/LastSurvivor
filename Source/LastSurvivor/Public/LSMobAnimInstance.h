// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Engine/DataTable.h"
#include "LSMobAnimInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnAttackDelegate);
DECLARE_DELEGATE(FOnDeathDelegate);
DECLARE_DELEGATE(FOnDestroyDelegate);
DECLARE_DELEGATE(FOnPlaySoundByStateDelegate);

UENUM(BlueprintType)
enum EMobType
{
	TypeA UMETA(DisplayName = "TypeA"), 
	TypeB UMETA(DisplayName = "TypeB"),
	TypeC UMETA(DisplayName = "TypeC")
};

UCLASS()
class LASTSURVIVOR_API ULSMobAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	FName GetMobTypeAsName(EMobType EnumValue);
	FOnAttackDelegate OnAttack;
	FOnDeathDelegate OnDeath;
	FOnDestroyDelegate OnDestroy;
	FOnPlaySoundByStateDelegate OnPlaySoundByState;
	
	ULSMobAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayHitMontage();
	void PlayDeathMontage();
	void PlayAttackMontage();
	void SetMobType(TEnumAsByte<EMobType> NewMobType);



private:
	UPROPERTY(EditAnywhere,  Category=AnimMontage)
	TArray<UAnimMontage*> HitMontages;

	UPROPERTY(EditAnywhere, Category=AnimMontage)
	TArray<UAnimMontage*> DeathMontages;
	
	UPROPERTY(EditAnywhere, Category=AnimMontage)
	TArray<UAnimMontage*> AttackMontages;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Character, meta=(AllowPrivateAccess=true))
	TEnumAsByte<EMobType> MobType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess=true))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Character, meta=(AllowPrivateAccess=true))
	bool bIsDead;
	
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	
	UFUNCTION()
	void AnimNotify_Death();

	UFUNCTION()
	void AnimNotify_Destroy();

	UFUNCTION()
	void AnimNotify_PlaySoundByState();
};
