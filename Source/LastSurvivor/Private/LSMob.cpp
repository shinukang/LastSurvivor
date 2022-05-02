#include "LSMob.h"
#include "DrawDebugHelpers.h"
#include "LastSurvivor.h"
#include "LSAIController.h"
#include "LSCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ALSMob::ALSMob()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetCapsuleSize(30.0f, 100.0f, true);
	GetMesh()->SetCollisionProfileName(FName("Mob"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	ItemSpawner = CreateDefaultSubobject<ULSItemSpawner>(TEXT("ItemSpawner"));

	DataTable = CreateDefaultSubobject<UDataTable>(TEXT("DataTable"));
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Mob(TEXT("DataTable'/Game/Mob/MobDataTable.MobDataTable'"));
	if(DT_Mob.Succeeded())
	{
		DataTable = DT_Mob.Object;
	}

	if(DataTable)
	{
		const int32 RandIndex = UKismetMathLibrary::RandomIntegerInRange(1, 6);

		const FName MobID = FName(FString(TEXT("M00") + FString::FormatAsNumber(RandIndex)));

		const FMobData* MobData = DataTable->FindRow<FMobData>(MobID, FString("Warn"));

		if(MobData)
		{
			GetMesh()->SetSkeletalMesh(MobData->MobMesh);
			GetCharacterMovement()->MaxWalkSpeed = MobData->Speed;

			MaxHealthPoint = MobData->MaxHealthPoint;
			CurrentHealthPoint = MaxHealthPoint;
			Damage = MobData->Damage;

			WalkSound = MobData->WalkSound;
			IdleSound = MobData->IdleSound;
			AttackSound = MobData->AttackSound;
			DeathSound = MobData->DeathSound;
			HitSound = MobData->HitSound;
		}
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> IDLE_ANIM(TEXT("/Game/Mob/LSMobAnimInstance.LSMobAnimInstance_C"));
	if(IDLE_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(IDLE_ANIM.Class);
	}

	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PSComp"));
	HitEffect->SetupAttachment(RootComponent);
	HitEffect->bAutoActivate = false;

	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HIT_EFFECT(TEXT("ParticleSystem'/Game/BallisticsVFX/Particles/Impacts/DynamicImpacts/Flesh/Blood_cloud_large.Blood_cloud_large'"));
	if(PS_HIT_EFFECT.Succeeded())
	{
		HitEffect->SetTemplate(PS_HIT_EFFECT.Object);
	}
	
	bIsDead = false;
	bIsHit = false;
	bIsAttack = false;
	
	AttackRange = 200.0f;
	DetectRange = 600.0f;
	
	AIControllerClass = ALSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ALSMob::BeginPlay()
{
	Super::BeginPlay();
}

void ALSMob::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LSMobAnim = Cast<ULSMobAnimInstance>(GetMesh()->GetAnimInstance());
	LSCHECK(nullptr != LSMobAnim);
	LSMobAnim->SetMobType(static_cast<EMobType>(FMath::RandRange(0, 2)));
	LSMobAnim->OnAttack.BindUFunction(this, "AttackHitCheck");
	LSMobAnim->OnDeath.BindUFunction(this, "SetNoReaction");
	LSMobAnim->OnDestroy.BindUFunction(this, "DropItemsAndDestroy");
	LSMobAnim->OnPlaySoundByState.BindUFunction(this, "PlaySoundByState");
}

void ALSMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALSMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ALSMob::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	bIsHit = true;
	DetectRange = 3000.0f;
	
	float TakenDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const auto PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		const auto HitResult = PointDamageEvent->HitInfo;
		const FName BoneName = HitResult.BoneName;
		
		if(0 == BoneName.Compare(FName(TEXT("Head"))))
		{
			TakenDamage *= 2;
		}
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect->Template, HitResult.ImpactPoint, UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
	}

	if(DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		TakenDamage *= 1.2f;
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	LSMobAnim->PlayHitMontage();
	CurrentHealthPoint -= TakenDamage;

	if(CurrentHealthPoint <= 0.0f)
	{
		bIsDead = true;
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		LSMobAnim->PlayDeathMontage();
	}
	return TakenDamage;
}

void ALSMob::Attack()
{
	if(!bIsAttack)
	{
		bIsAttack = true;
		LSMobAnim->PlayAttackMontage();
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
	}
}

void ALSMob::StopAttack()
{
	bIsAttack = false;
	LSMobAnim->StopAllMontages(0.1f);
}

void ALSMob::AttackHitCheck()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * AttackRange;
	FHitResult HitResult;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
		
	FCollisionObjectQueryParams CollisionObjectQueryParams;

	if(GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, CollisionObjectQueryParams, CollisionQueryParams))
	{
		if(HitResult.Actor->IsA(ALSCharacter::StaticClass()))
		{
			UGameplayStatics::ApplyDamage(Cast<ALSCharacter>(HitResult.Actor), 20.0f, nullptr, this, nullptr);
		}
	}
}

void ALSMob::SetNoReaction()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetController()->UnPossess();
}

void ALSMob::DropItemsAndDestroy()
{
	ItemSpawner->SpawnAllItems(GetMesh()->GetBoneLocation(FName("Root")));
	Destroy();
}


bool ALSMob::IsDead()
{
	return bIsDead;
}

bool ALSMob::IsHit()
{
	return bIsHit;
}

float ALSMob::GetHealthPoint()
{
	return CurrentHealthPoint;
}

float ALSMob::GetDetectRange()
{
	return DetectRange;
}

float ALSMob::GetAttackRange()
{
	return AttackRange;
}

void ALSMob::PlaySoundByState()
{
	const auto StateMachineIndex = LSMobAnim->GetStateMachineIndex(FName("Locomotion"));
	
	const auto State = LSMobAnim->GetCurrentStateName(StateMachineIndex);
	
	if(State == FName("Idle"))
	{
		UGameplayStatics::PlaySoundAtLocation(this, IdleSound, GetActorLocation());
	}
	else if(State == FName("Walk"))
	{
		UGameplayStatics::PlaySoundAtLocation(this, WalkSound, GetActorLocation());
	}
}




