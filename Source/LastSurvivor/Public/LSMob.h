// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSAIController.h"
#include "LSItemSpawner.h"
#include "LSMobAnimInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "LSMob.generated.h"


USTRUCT(BlueprintType)
struct FMobData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* MobMesh; // 외관
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealthPoint; // 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed; // 이동속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage; // 공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* IdleSound;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* WalkSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* DeathSound;
};

UCLASS()
class LASTSURVIVOR_API ALSMob : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALSMob();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void Attack();

	UFUNCTION()
	void AttackHitCheck();
	
	void StopAttack();

	UFUNCTION()
	void SetNoReaction();

	UFUNCTION()
	void DropItemsAndDestroy();

	UFUNCTION()
	void PlaySoundByState();

	bool IsDead();
	bool IsHit();
	
	float GetHealthPoint();
	float GetDetectRange();
	float GetAttackRange();

	UDataTable* DataTable;
	
	
	UPROPERTY()
	ULSItemSpawner* ItemSpawner;

private:

	bool bIsDead;
	bool bIsHit;
	bool bIsAttack;
	
	float MaxHealthPoint;
	float CurrentHealthPoint;
	float DetectRange;
	float AttackRange;
	float Speed;
	float Damage;

	USoundCue* IdleSound;

	USoundCue* HitSound;

	USoundCue* AttackSound;

	USoundCue* WalkSound;
	
	USoundCue* DeathSound;

	UParticleSystemComponent* HitEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ULSMobAnimInstance* LSMobAnim;
};
