#pragma once

#include "CoreMinimal.h"
#include "LSInventory.h"
#include "MatineeCameraShake.h"
#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "LSGrenade.generated.h"

USTRUCT(BlueprintType)
struct FGrenadeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSlateBrush Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* GrenadeMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* ThrowSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
};

UCLASS()
class LASTSURVIVOR_API ALSGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSGrenade();

	UPROPERTY(VisibleAnywhere, Category=Collision)
	USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere, Category=Mesh)
	USkeletalMeshComponent* GrenadeMeshComp;
	
	UPROPERTY(VisibleAnywhere, Category=Movement)
	UProjectileMovementComponent* MovementComp;

	FTimerHandle ExplosionTimer;

	UPROPERTY()
	TSubclassOf<UMatineeCameraShake> CameraShake_Explosion;

	void ThrowInDirection(FVector ShootDirection);
	void Initialize(FName GrenadeID);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UDataTable* DataTable;
	
	UPROPERTY()
	USkeletalMesh* GrenadeMesh;
	
	UPROPERTY()
	UParticleSystem* ExplosionEffect;

	UPROPERTY()
	USoundCue* ExplosionSound;

	UPROPERTY()
	USoundCue* ThrowSound;

	float Damage;
	
	void Explosion();
};
