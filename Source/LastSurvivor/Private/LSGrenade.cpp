#include "LSGrenade.h"
#include "LSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ALSGrenade::ALSGrenade()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Grenade"));
	CollisionComp->SetSphereRadius(5.0f);
	
	RootComponent = CollisionComp;
	
	GrenadeMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GRENADE"));
	GrenadeMeshComp->SetupAttachment(RootComponent);
	GrenadeMeshComp->BodyInstance.SetCollisionProfileName(TEXT("Grenade"));
	GrenadeMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MOVEMENT"));
	MovementComp->bShouldBounce = true;
	MovementComp->Bounciness = 0.1f;
	MovementComp->InitialSpeed = 0.0f;
	MovementComp->SetUpdatedComponent(RootComponent);

	MovementComp->ProjectileGravityScale = 1.2f;

	static ConstructorHelpers::FClassFinder<UMatineeCameraShake> CS_Explosion(TEXT("/Game/HUD/CamShake_Explosion.CamShake_Explosion_C"));
	if(CS_Explosion.Succeeded())
	{
		CameraShake_Explosion = CS_Explosion.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Grenade(TEXT("DataTable'/Game/Grenade/GrenadeDataTable.GrenadeDataTable'"));
	if(DT_Grenade.Succeeded())
	{
		DataTable = DT_Grenade.Object;
	}
}

void ALSGrenade::BeginPlay()
{
	Super::BeginPlay();
}


void ALSGrenade::Initialize(FName GrenadeName)
{
	if(DataTable)
	{
		const FGrenadeData* Data = DataTable->FindRow<FGrenadeData>(GrenadeName, FString("Warn"));
		if(Data)
		{
			GrenadeMeshComp->SetSkeletalMesh(Data->GrenadeMesh);
			ExplosionEffect = Data->ExplosionEffect;
			ExplosionSound = Data->ExplosionSound;
			ThrowSound = Data->ThrowSound;
			Damage = Data->Damage;
		}
		else
		{
			LSLOG_S(Error);
		}
	}
}

void ALSGrenade::ThrowInDirection(FVector ShootDirection)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ThrowSound, GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimer, this, &ALSGrenade::Explosion, 2.0f, false); 
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
}

void ALSGrenade::Explosion()
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), 1000.0f, nullptr, TArray<AActor*>(), this, nullptr, true, ECC_Visibility);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake_Explosion, GetActorLocation(), 500.0f, 3000.0f);
	Destroy();
}
