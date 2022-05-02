#pragma once

#include "LastSurvivor.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "LSAmmo.generated.h"

USTRUCT(BlueprintType)
struct FAmmoFX : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* OnHitEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* OnHitSound;
};

USTRUCT(BlueprintType)
struct FAmmoData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* AmmoMesh;
};

UCLASS()
class LASTSURVIVOR_API ALSAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSAmmo();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, Category=MeshComp)
	UStaticMeshComponent* AmmoMeshComp;
	
	UPROPERTY(VisibleAnywhere, Category=Movement)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY()
	UDataTable* AmmoDataTable;

	UPROPERTY()
	UDataTable* AmmoFXDataTable;

	float Damage;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void Initialize(FName AmmoID);

	void FireInDirection(const FVector& ShootDirection, float Damage);
};
