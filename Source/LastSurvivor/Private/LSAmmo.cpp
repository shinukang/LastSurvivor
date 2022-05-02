#include "LastSurvivor.h"
#include "LSAmmo.h"
#include "LSMob.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ALSAmmo::ALSAmmo()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AmmoMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AMMO"));
	AmmoMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	AmmoMeshComp->BodyInstance.SetCollisionProfileName(TEXT("Ammo"));
	AmmoMeshComp->OnComponentHit.AddDynamic(this, &ALSAmmo::OnHit);
	RootComponent = AmmoMeshComp;
	
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MOVEMENT"));
	MovementComp->SetUpdatedComponent(RootComponent);
	MovementComp->InitialSpeed = 10000.0f;
	MovementComp->MaxSpeed = 10000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = false;
	MovementComp->ProjectileGravityScale = 0.0f;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_AMMO(TEXT("DataTable'/Game/Weapons/AmmoDataTable.AmmoDataTable'"));
	if(DT_AMMO.Succeeded())
	{
		AmmoDataTable = DT_AMMO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_AMMO_FX(TEXT("DataTable'/Game/Weapons/AmmoFXDataTable.AmmoFXDataTable'"));
	if(DT_AMMO_FX.Succeeded())
	{
		AmmoFXDataTable = DT_AMMO_FX.Object;
	}
	
}

void ALSAmmo::BeginPlay()
{
	Super::BeginPlay();
}

void ALSAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALSAmmo::FireInDirection(const FVector& ShootDirection, float WeaponDamage)
{
	MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	Damage = WeaponDamage;
}

void ALSAmmo::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->IsA(ALSMob::StaticClass()))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetActorLocation(), Hit, NULL, GetOwner(), NULL);
		Destroy();
		return;
	}
	
	FHitResult Result;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	
	if(GetWorld()->SweepSingleByChannel(Result, Hit.Location, Hit.Location, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(3.0f), CollisionQueryParams))
	{
		auto SurfaceType = Result.PhysMaterial->SurfaceType;

		FName RowName = SurfaceTypeToName(SurfaceType);

		if(AmmoFXDataTable)
		{
			FAmmoFX* AmmoFXData = AmmoFXDataTable->FindRow<FAmmoFX>(RowName, FString("Warn"));

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AmmoFXData->OnHitSound, Result.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AmmoFXData->OnHitEffect,Result.ImpactPoint, UKismetMathLibrary::MakeRotFromZ(Result.ImpactNormal));
		}
		else
		{
			LSLOG_S(Error);
		}
	}
	Destroy();
}

void ALSAmmo::Initialize(FName AmmoID)
{
	if(AmmoDataTable)
	{
		const FAmmoData* AmmoData = AmmoDataTable->FindRow<FAmmoData>(AmmoID, FString("Warn"));
		
		if(AmmoData)
		{
			AmmoMeshComp->SetStaticMesh(AmmoData->AmmoMesh);
		}
	}
}




