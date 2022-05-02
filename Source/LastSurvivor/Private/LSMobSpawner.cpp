#include "LSMobSpawner.h"
#include "LSMob.h"

ALSMobSpawner::ALSMobSpawner()
{
	RootComponent = MeshComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SpawnerMesh(TEXT("StaticMesh'/Game/MS_MilitaryEquipment/3D_Assets/Military_Wooden_Crate_00/ud4nfhofa_LOD0.ud4nfhofa_LOD0'"));
	if(SpawnerMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(SpawnerMesh.Object);
	}
	
	NumOfMobs = 0;
}

void ALSMobSpawner::BeginPlay()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ALSMobSpawner::SpawnMobs, 3.0f);
}

void ALSMobSpawner::SpawnMobs()
{
	if(NumOfMobs < MAX_NUM_OF_MOB)
	{
		GetWorld()->SpawnActor<ALSMob>(GetActorLocation(), GetActorRotation());
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ALSMobSpawner::SpawnMobs, 5.0f);	
	}
	else
	{
		Destroy();
	}
}


