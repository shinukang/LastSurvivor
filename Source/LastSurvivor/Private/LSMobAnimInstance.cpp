// Fill out your copyright notice in the Description page of Project Settings.


#include "LSMobAnimInstance.h"
#include "LSCharacter.h"
#include "LSMob.h"

ULSMobAnimInstance::ULSMobAnimInstance()
{
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontage01(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Hit_1.AM_Zombie_Hit_1'"));
	if(HitMontage01.Succeeded())
	{
		HitMontages.Add(HitMontage01.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontage02(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Hit_2.AM_Zombie_Hit_2'"));
	if(HitMontage02.Succeeded())
	{
		HitMontages.Add(HitMontage02.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontage03(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Hit_3.AM_Zombie_Hit_3'"));
	if(HitMontage03.Succeeded())
	{
		HitMontages.Add(HitMontage03.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontage04(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Hit_4.AM_Zombie_Hit_4'"));
	if(HitMontage04.Succeeded())
	{
		HitMontages.Add(HitMontage04.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontage05(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Hit_5.AM_Zombie_Hit_5'"));
	if(HitMontage05.Succeeded())
	{
		HitMontages.Add(HitMontage05.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontage01(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Death_1.AM_Zombie_Death_1'"));
	if(DeathMontage01.Succeeded())
	{
		DeathMontages.Add(DeathMontage01.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontage02(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Death_2.AM_Zombie_Death_2'"));
	if(DeathMontage02.Succeeded())
	{
		DeathMontages.Add(DeathMontage02.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontage03(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Death_3.AM_Zombie_Death_3'"));
	if(DeathMontage03.Succeeded())
	{
		DeathMontages.Add(DeathMontage03.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage01(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Attack_1.AM_Zombie_Attack_1'"));
	if(AttackMontage01.Succeeded())
	{
		AttackMontages.Add(AttackMontage01.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage02(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Attack_2.AM_Zombie_Attack_2'"));
	if(AttackMontage02.Succeeded())
	{
		AttackMontages.Add(AttackMontage02.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage03(TEXT("AnimMontage'/Game/Mob/Animations/Montages/AM_Zombie_Attack_3.AM_Zombie_Attack_3'"));
	if(AttackMontage03.Succeeded())
	{
		AttackMontages.Add(AttackMontage03.Object);
	}
}

void ULSMobAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	if(IsValid(Character))
	{
		Speed = Character->GetVelocity().Size();
		ALSMob* LSMob = Cast<ALSMob>(Character);
		if(IsValid(LSMob))
		{
			bIsDead = LSMob->IsDead();
		}
	}
}

void ULSMobAnimInstance::PlayHitMontage()
{
	auto const Index = FMath::RandRange(0, HitMontages.Num() - 1);
	Montage_Play(HitMontages[Index]);
}

void ULSMobAnimInstance::PlayDeathMontage()
{
	auto const Index = FMath::RandRange(0, DeathMontages.Num() - 1);

	if(IsAnyMontagePlaying())
	{
		StopAllMontages(0.1f);
	}
	Montage_Play(DeathMontages[Index]);
}

void ULSMobAnimInstance::PlayAttackMontage()
{
	auto const Index = FMath::RandRange(0, AttackMontages.Num() - 1);

	if(!IsAnyMontagePlaying())
	{
		Montage_Play(AttackMontages[Index]);
	}
}

FName ULSMobAnimInstance::GetMobTypeAsName(EMobType EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMobType"), true);
	if(nullptr == EnumPtr)
	{
		return FName("Invalid");
	}
	return FName(EnumPtr->GetDisplayNameTextByIndex(EnumValue).ToString());
}

void ULSMobAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttack.Execute();
}

void ULSMobAnimInstance::AnimNotify_Death()
{
	OnDeath.Execute();
}

void ULSMobAnimInstance::AnimNotify_Destroy()
{
	OnDestroy.Execute();
}

void ULSMobAnimInstance::AnimNotify_PlaySoundByState()
{
	if(!IsAnyMontagePlaying())
	{
		OnPlaySoundByState.Execute();
	}
}

void ULSMobAnimInstance::SetMobType(TEnumAsByte<EMobType> NewMobType)
{
	MobType = NewMobType;
}
