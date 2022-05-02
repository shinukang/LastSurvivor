#include "LSCharacter.h"
#include "LSWeapon.h"
#include "Engine.h"
#include "LSGrenade.h"
#include "LSHealthKit.h"
#include "LSPlayerController.h"
#include "Blueprint/UserWidget.h"

ALSCharacter::ALSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("Character"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -92.5f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("SkeletalMesh'/Game/Character/Meshes/SM_Soldier.SM_Soldier'"));
	if(SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> IDLE_ANIM(TEXT("/Game/Character/Animations/LSAnimInstance.LSAnimInstance_C"));
	if(IDLE_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(IDLE_ANIM.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	SetControlMode();

	Inventory = CreateDefaultSubobject<ULSInventory>(TEXT("Inventory"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CV_AIMING(TEXT("CurveFloat'/Game/HUD/AimingCurve.AimingCurve'"));    
	if(CV_AIMING.Succeeded())
	{
		AimingCurve = CV_AIMING.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CV_BLOOD_FRAME(TEXT("CurveFloat'/Game/HUD/BloodFrameCurve.BloodFrameCurve'"));    
	if(CV_BLOOD_FRAME.Succeeded())
	{
		BloodFrameCurve = CV_BLOOD_FRAME.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_FootStep(TEXT("DataTable'/Game/Character/DataTables/FootStepDataTable.FootStepDataTable'"));
	if(DT_FootStep.Succeeded())
	{
		FootStepDataTable = DT_FootStep.Object;
	}

	static ConstructorHelpers::FClassFinder<UMatineeCameraShake> CS_Hit(TEXT("/Game/HUD/CamShake_Hit.CamShake_Hit_C"));
	if(CS_Hit.Succeeded())
	{
		HitCameraShake = CS_Hit.Class;
	}

	UMaterialInterface* Material = GetMesh()->GetMaterial(0);
	MatInstance = GetMesh()->CreateDynamicMaterialInstance(0, Material);
	
	GrenadeDelay = 10.0f;
	MaxHealthPoint = 1000.0f;
	CurrentHealthPoint = MaxHealthPoint * 0.3;
	BloodFrameAlpha = 0.0f;
}

void ALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ALSWeapon* NewWeapon = GetWorld()->SpawnActor<ALSWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	NewWeapon->Initialize(FName("W001"));
	Equip(NewWeapon);
	Inventory->AddToInventory(NewWeapon);	

	CrossHairLocation = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X / 2.0f, GEngine->GameViewport->Viewport->GetSizeXY().Y / 2.0f);

	if(AimingCurve)
	{
		FOnTimelineFloat TimelineCallbackFunc;
		TimelineCallbackFunc.BindUFunction(this, FName("AimingProgress"));
		AimingTimeline.AddInterpFloat(AimingCurve, TimelineCallbackFunc);
		AimingTimeline.SetLooping(false);
	}

	if(BloodFrameCurve)
	{
		FOnTimelineFloat BloodFrameCallback;
		BloodFrameCallback.BindUFunction(this, FName("BloodFrameProgress"));
		BloodFrameTimeline.AddInterpFloat(BloodFrameCurve, BloodFrameCallback);
		BloodFrameTimeline.SetLooping(false);
	}
}

void ALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimingTimeline.TickTimeline(DeltaTime);
	BloodFrameTimeline.TickTimeline(DeltaTime);
}

void ALSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LSAnim = Cast<ULSAnimInstance>(GetMesh()->GetAnimInstance());
	LSCHECK(nullptr != LSAnim);
	
	LSAnim->OnFire.BindUFunction(this, FName("Fire"));
	LSAnim->OnThrow.BindUFunction(this, FName("Throw"));
	LSAnim->OnEquip.BindUFunction(this, FName("Equip"));
	LSAnim->OnFootStep.BindUFunction(this, FName("PlayFootStep"));
	LSAnim->OnMontageEnded.AddDynamic(this, &ALSCharacter::OnMontageEnded);
}

void ALSCharacter::PossessedBy(AController* NewController)
{
	LSLOG_S(Warning);
	Super::PossessedBy(NewController);
}

void ALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed,this, &ALSCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ALSCharacter::StartRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ALSCharacter::StopRun);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ALSCharacter::TryReload);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ALSCharacter::TryFire);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &ALSCharacter::StopFire);
	PlayerInputComponent->BindAction(TEXT("Throw"), IE_Released, this, &ALSCharacter::TryThrow);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ALSCharacter::StartAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ALSCharacter::StopAim);
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ALSCharacter::OpenInventory);
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Released, this, &ALSCharacter::CloseInventory);
	
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ALSCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ALSCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ALSCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ALSCharacter::Turn);
}

void ALSCharacter::SetControlMode()
{
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 100.0f), FRotator(0.0f, -40.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
	GetCharacterMovement()->bCanWalkOffLedges = true;
	JumpMaxCount = 1;
}

float ALSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealthPoint -= Damage;

	BloodFrameTimeline.PlayFromStart();

	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
	
	return Damage;
}

void ALSCharacter::UpDown(float NewAxisValue)
{
	if(!IsInWidget) AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void ALSCharacter::LeftRight(float NewAxisValue)
{
	if(!IsInWidget) AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void ALSCharacter::LookUp(float NewAxisValue)
{
	if(!IsInWidget) AddControllerPitchInput(NewAxisValue);
}

void ALSCharacter::Turn(float NewAxisValue)
{
	if(!IsInWidget) AddControllerYawInput(NewAxisValue);
}

void ALSCharacter::OpenInventory()
{
	FindItemsOnGround();
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("OpenInven"));
	auto LSController = Cast<ALSPlayerController>(GetController());

	LSController->CreateInventoryWidget();
	IsInWidget = true;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("IsInWidget = true"));
}

void ALSCharacter::CloseInventory()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("CloseInven"));

	Inventory->ClearItemsOnGround();
	const auto LSController = Cast<ALSPlayerController>(GetController());

	LSController->DestroyInventoryWidget();
	IsInWidget = false;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("IsInWidget = false"));
}

void ALSCharacter::FindItemsOnGround()
{
	TArray<FOverlapResult> Results;
	const FVector Center = GetActorLocation();
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(TArray<AActor*>{this, Inventory->GetCurrentWeapon()});
	
	if(GetWorld()->OverlapMultiByProfile(Results, Center, FQuat::Identity, FName("Item"), FCollisionShape::MakeSphere(450.0f), CollisionQueryParams))
	{
		for(FOverlapResult Result : Results)
		{
			Inventory->AddToGround(Cast<ALSItem>(Result.Actor));
		}
	}
}

void ALSCharacter::TryFire()
{
	LSLOG_S(Warning);
	if(CanFire())
	{
		IsFiring = true;
		AimingTimeline.Play();
		OnFire();
	}
}

void ALSCharacter::OnFire()
{
	if(IsFiring && CanFire())
	{
		LSLOG_S(Warning);

		LSAnim->PlayFireMontage();

		GetWorld()->GetTimerManager().SetTimer(WeaponTimer, this, &ALSCharacter::OnFire, Inventory->GetCurrentWeapon()->GetRateOfFire(), false); // 연사를 구현한다.

		if(Inventory->GetCurrentWeapon()->GetCurrentAmmo() > 0)
		{
			GetWorldTimerManager().SetTimer(Inventory->GetCurrentWeapon()->ReFireCheckTimer, Inventory->GetCurrentWeapon()->GetRateOfFire(), false); // 연사가 끝나면 발사 딜레이만큼 타이머를 설정한다. (이 시간 동안 발사 불가능)
		}
		else
		{
			StopFire();
		}
	}
}

void ALSCharacter::Fire()
{
	Inventory->GetCurrentWeapon()->Fire(GetFireDirection());
}

void ALSCharacter::StopFire()
{
	LSLOG_S(Warning);
	IsFiring = false;
	if(!IsAiming) AimingTimeline.Reverse();
}

void ALSCharacter::StartAim()
{
	CrossHairLocation = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X / 2.0f, GEngine->GameViewport->Viewport->GetSizeXY().Y / 2.0f);

	if(!IsInWidget)
	{
		IsAiming = true;
		AimingTimeline.Play();	
	}
}

void ALSCharacter::StopAim()
{
	if(!IsInWidget)
	{
		IsAiming = false;
		AimingTimeline.Reverse();
	}
}

void ALSCharacter::TryThrow()
{
	if(CanThrow())
	{
		IsThrowing = true;
		LSAnim->PlayThrowMontage();
	}
}

void ALSCharacter::Throw()
{
	const FVector StartLocation = GetMesh()->GetSocketLocation(FName("GrenadeSocket")); 
	const FVector GoalDirection = GetFireDirection();
		
	ALSGrenade* Grenade = GetWorld()->SpawnActor<ALSGrenade>(StartLocation, FRotator::ZeroRotator);
	
	if(IsValid(Grenade))
	{
		GetWorldTimerManager().SetTimer(GrenadeTimer, GrenadeDelay, false);
		Grenade->Initialize("Frag");
		Grenade->ThrowInDirection(GoalDirection);
	}
}

void ALSCharacter::TryReload()
{
	if(CanReload())
	{
		IsReloading = true;
		
		LSAnim->PlayReloadMontage();
		
		Inventory->GetCurrentWeapon()->PlayReloadSound();
	}
}

void ALSCharacter::Reload()
{
	Inventory->GetCurrentWeapon()->Reload();	
}

void ALSCharacter::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
}

void ALSCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
}

void ALSCharacter::TryEquip(FItemData WeaponData)
{
	IsEquipping = true;

	ALSWeapon* CurrentWeapon = Inventory->GetCurrentWeapon();
	
	if(IsValid(CurrentWeapon))
	{
		Inventory->GetCurrentWeapon()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Inventory->GetCurrentWeapon()->Destroy();
	}

	ALSWeapon* NewWeapon = GetWorld()->SpawnActor<ALSWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	NewWeapon->SetActorHiddenInGame(true);
	NewWeapon->Initialize(WeaponData);
	
	if(nullptr != NewWeapon) 
	{
		FName WeaponSocket; 
		
		if(NewWeapon->GetWeaponType() == Pistol)
		{
			WeaponSocket = FName(TEXT("Pistol_Socket"));
		}
		else
		{
			WeaponSocket = FName(TEXT("Rifle_Socket"));
		}
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		Inventory->SetCurrentWeapon(NewWeapon);
	}
	LSAnim->PlayEquipMontage();
}

void ALSCharacter::Equip(ALSWeapon* NewWeapon)
{
	if(Inventory->GetCurrentWeapon()) 
	{
		Inventory->GetCurrentWeapon()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Inventory->GetCurrentWeapon()->Destroy();
	}
	
	if(IsValid(NewWeapon)) 
	{
		FName WeaponSocket; 
		
		if(NewWeapon->GetWeaponType() == Pistol)
		{
			WeaponSocket = FName(TEXT("Pistol_Socket"));
		}
		else
		{
			WeaponSocket = FName(TEXT("Rifle_Socket"));
		}
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket); 
		Inventory->SetCurrentWeapon(NewWeapon);
	}
}

void ALSCharacter::Equip()
{
	Inventory->GetCurrentWeapon()->SetActorHiddenInGame(false);
}

void ALSCharacter::AimingProgress(float Value)
{
	Camera->SetRelativeLocation(FVector(Value * 100.0f, Value * 30.0f, 0.0f));
	Camera->SetFieldOfView(90.0f - Value * 20.0f);
}

void ALSCharacter::BloodFrameProgress(float Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%f"), Value));
	BloodFrameAlpha = Value;
}

FVector ALSCharacter::GetCrossHairImpactPoint()
{
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrossHairLocation,
		CrossHairWorldPosition,
		CrossHairWorldDirection);

	if(bScreenToWorld)
	{
		const FVector Start { CrossHairWorldPosition };
		const FVector End { CrossHairWorldPosition + CrossHairWorldDirection * 100000.0f };
		FHitResult HitResult;
		
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams CollisionObjectQueryParams;
		
		if(GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, CollisionObjectQueryParams, CollisionQueryParams))
		{
			return HitResult.ImpactPoint;
		}
		LSLOG(Warning, TEXT("Return End Point"));
		return End;;
	}
	LSLOG_S(Error);
	return FVector::ZeroVector;
}

FVector ALSCharacter::GetFireDirection()
{
	const FVector CrossHairImpactPoint = GetCrossHairImpactPoint();
	if(!CrossHairImpactPoint.Equals(FVector::ZeroVector))
	{
		const FVector Start = Inventory->GetCurrentWeapon()->GetMesh()->GetSocketLocation("Barrel_Squared");
		const FVector End = CrossHairImpactPoint;
		const FVector Direction = End - Start;
		const FVector GoalDirection = Direction.GetSafeNormal();
		return GoalDirection;
	}
	LSLOG_S(Error);
	return FVector::ZeroVector;
}

void ALSCharacter::PlayFootStep(TEnumAsByte<EFootStepState> State)
{

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, 150.0f);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	
	if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionQueryParams))
	{
		auto SurfaceType = HitResult.PhysMaterial->SurfaceType;
		FName RowName = SurfaceTypeToName(SurfaceType);

		if(IsValid(FootStepDataTable))
		{
			FFootStepFX* Data = FootStepDataTable->FindRow<FFootStepFX>(RowName, FString("Warn"));

			switch(State)
			{
			case FSS_Left:
				UGameplayStatics::PlaySoundAtLocation(this, Data->LeftFootStepSound, GetActorLocation());
				break;
			case FSS_Right:
				UGameplayStatics::PlaySoundAtLocation(this, Data->RightFootStepSound, GetActorLocation());
				break;
			case FSS_Land:
				UGameplayStatics::PlaySoundAtLocation(this, Data->LandFootStepSound, GetActorLocation());
				break;
				
				default:
					break;
			}
		}
		else
		{
			LSLOG_S(Error);
		}
	}
}

void ALSCharacter::Recovery(FItemData HealthKitData)
{
	int32 Index;
	
	if(Inventory->InventoryItems.Find(HealthKitData, Index))
	{
		ALSHealthKit* HealthKit = GetWorld()->SpawnActor<ALSHealthKit>(FVector::ZeroVector, FRotator::ZeroRotator);

		HealthKit->Initialize(HealthKitData.ID);
		HealthKit->SetActorHiddenInGame(true);
		
		CurrentHealthPoint += MaxHealthPoint * HealthKit->RecoveryRate;

		if(CurrentHealthPoint > MaxHealthPoint)
		{
			CurrentHealthPoint = MaxHealthPoint;
		}
		Inventory->UseItem(Index);
		HealthKit->Destroy();
	}
}

float ALSCharacter::GetHealthPointRate()
{
	return CurrentHealthPoint / MaxHealthPoint;
}

float ALSCharacter::GetGrenadeRemainingRate()
{
	return GetWorldTimerManager().GetTimerRemaining(GrenadeTimer) / GrenadeDelay;
}

void ALSCharacter::SetMaterialTexture(UTexture2D* Texture)
{
	MatInstance->SetTextureParameterValue(TEXT("Texture"), Texture);
}

void ALSCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(LSAnim->IsReloadMontage(Montage))
	{
		Reload();
		IsReloading = false;
	}
	else if(LSAnim->IsEquipMontage(Montage))
	{
		IsEquipping = false;
	}
	else if(LSAnim->IsThrowMontage(Montage))
	{
		IsThrowing = false;
	}
}

bool ALSCharacter::CanFire()
{
	return !IsInWidget && !IsEquipping && !IsThrowing && !IsReloading && GetWorldTimerManager().GetTimerRemaining(Inventory->GetCurrentWeapon()->ReFireCheckTimer) <= 0.0f;
}

bool ALSCharacter::CanReload()
{
	return !IsInWidget && !IsEquipping && !IsThrowing && !IsReloading;
}

bool ALSCharacter::CanThrow()
{
	return !IsInWidget && !IsEquipping && !IsThrowing && !IsReloading && GetWorldTimerManager().GetTimerRemaining(GrenadeTimer) <= 0.0f;
}

bool ALSCharacter::CanEquip()
{
	return !IsInWidget && !IsFiring && !IsThrowing && !IsReloading && !IsEquipping;
}



