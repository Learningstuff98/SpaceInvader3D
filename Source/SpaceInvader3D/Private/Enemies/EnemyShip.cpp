#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "PatrolTargets/PatrolTarget.h"
#include "Projectiles/BlasterShot.h"
#include "Statics/ShipStatics.h"
#include "Components/BoxComponent.h"
#include "PlayerShip/PlayerShip.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "Statics/ShipStatics.h"
#include "Components/ArrowComponent.h"

#include "Development/Development.h"

AEnemyShip::AEnemyShip() {
	PrimaryActorTick.bCanEverTick = true;
	DetectedPlayerShip = nullptr;
	DetectedPlayerShipNullOutTimerFinished = true;
	HideLockedOnUIBoxTimerFinished = true;
	TurnSpeed = 3.f;
	NewPatrolTargetIndex = 0;
	CurrentPatrolTargetIndex = 0;
	Health = 500;
	PlayEngineSoundTimerFinished = true;
	AimedAtPlayerShip = nullptr;
	NullOutAimedAtPlayerShipTimerFinished = true;
	BlasterShotReloadTimerFinished = true;
	RightBarrelHasFired = false;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	SetRootComponent(ShipMesh);
	ShipMesh->SetNotifyRigidBodyCollision(true);
	ShipMesh->SetCollisionProfileName(FName("Custom"));
	ShipMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ShipMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ShipMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	ShipMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SetPeripheralVisionAngle(70.0f);

	AimingSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Aiming Sensing Component"));
	AimingSensingComponent->SetPeripheralVisionAngle(10.0f);

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement Component"));
	PawnMovementComponent->MaxSpeed = 5000.0f;

	FieldSystemSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Field System Spawn Location"));
	FieldSystemSpawnLocation->SetupAttachment(GetRootComponent());

	MissleLockOnUIBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Missle Lock On UI Box"));
	MissleLockOnUIBox->SetupAttachment(GetRootComponent());
	MissleLockOnUIBox->bHiddenInGame = false;

	EngineThrusterEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect"));
	EngineThrusterEffect->SetupAttachment(GetRootComponent());

	LeftGunBarrel = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Gun Barrel"));
	LeftGunBarrel->SetupAttachment(GetRootComponent());

	RightGunBarrel = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Gun Barrel"));
	RightGunBarrel->SetupAttachment(GetRootComponent());
}

void AEnemyShip::BeginPlay() {
	Super::BeginPlay();
	SetupPlayerShipDetection();
	SetMissleLockOnUIBoxVisibility(false);
}

void AEnemyShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleChasingRotation();
	AddMovementInput(GetActorForwardVector(), 1.0f);
	HandleDetectedPlayerShipNullOutTimer();
	HandleExploding();
	UpdateMissleLockOnUIBoxRotation();
	HandleHidingLockedOnUIBox();
	HandleEngineSound();
	HandleFiringBlasterShots();
	HandleNullingOutAimedAtPlayerShip();
}

void AEnemyShip::HandleChasingRotation() {
	if (DetectedPlayerShip) {
		SetActorRotation(GetChasingRotation(GetPlayerShipLookAtRotation()));
	} else if (PatrolTargets.Num() > 0) {
		SetActorRotation(GetChasingRotation(GetPatrolTargetLookAtRotation()));
	}
}

FRotator AEnemyShip::GetChasingRotation(const FRotator& LookAtRotation) {
	return UKismetMathLibrary::RInterpTo(
		GetActorRotation(),
		LookAtRotation,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		TurnSpeed
	);
}

FRotator AEnemyShip::GetPatrolTargetLookAtRotation() {
	return UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		PatrolTargets[CurrentPatrolTargetIndex]->GetActorLocation()
	);
}

FRotator AEnemyShip::GetPlayerShipLookAtRotation() {
	return UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		DetectedPlayerShip->GetActorLocation()
	);
}

void AEnemyShip::HandleDetectedPlayerShipNullOutTimer() {
	if (DetectedPlayerShipNullOutTimerFinished) {
		GetWorldTimerManager().ClearTimer(DetectedPlayerShipNullOutTimer);
		GetWorldTimerManager().SetTimer(DetectedPlayerShipNullOutTimer, this, &AEnemyShip::NullOutDetectedPlayerShip, 0.1f);
		DetectedPlayerShipNullOutTimerFinished = false;
	}
}

void AEnemyShip::NullOutDetectedPlayerShip() {
	DetectedPlayerShip = nullptr;
	DetectedPlayerShipNullOutTimerFinished = true;
}

void AEnemyShip::GetRandomPatrolTargetIndex() {
	while (CurrentPatrolTargetIndex == NewPatrolTargetIndex) {
		NewPatrolTargetIndex = FMath::RandRange(0, PatrolTargets.Num() - 1);
	}
	CurrentPatrolTargetIndex = NewPatrolTargetIndex;
}

void AEnemyShip::SetupPlayerShipDetection() {
	if (PawnSensingComponent) {
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyShip::SetDetectedPlayerShip);
	}
	if (AimingSensingComponent) {
		AimingSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyShip::SetAimedAtPlayerShip);
	}
}

void AEnemyShip::SetDetectedPlayerShip(APawn* SeenPawn) {
	if (SeenPawn) DetectedPlayerShip = SeenPawn;
}

void AEnemyShip::SetAimedAtPlayerShip(APawn* SeenPawn) {
	if (SeenPawn) AimedAtPlayerShip = SeenPawn;
}

void AEnemyShip::HandleExploding() {
	if (Health <= 0) {
		Explode();
		Destroy();
	}
}

void AEnemyShip::Explode() {
	ShipStatics::SpawnShipExplodingEffect(ShipExplodingEffectBlueprintClass, this);
	ShipStatics::SpawnShipPieces(ShipPiecesBlueprintClass, this);
	if (FieldSystemSpawnLocation && GetWorld()) {
		ShipStatics::SpawnShipExplodingFieldSystem(
			ShipExplodingFieldSystemBlueprintClass,
			GetWorld(),
			FieldSystemSpawnLocation->GetComponentLocation(),
			FieldSystemSpawnLocation->GetComponentRotation()
		);
	}
	if (ExplodingSound) ShipStatics::PlaySound(ExplodingSound, this);
}

void AEnemyShip::UpdateMissleLockOnUIBoxRotation() {
	if (MissleLockOnUIBox && PlayerShip) {
		MissleLockOnUIBox->SetWorldRotation(
			UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(),
				PlayerShip->GetCameraLocation()
			)
		);
	}
}

void AEnemyShip::HandleHidingLockedOnUIBox() {
	if (HideLockedOnUIBoxTimerFinished) {
		GetWorldTimerManager().ClearTimer(HideLockedOnUIBoxTimer);
		GetWorldTimerManager().SetTimer(HideLockedOnUIBoxTimer, this, &AEnemyShip::HideLockedOnUIBox, 0.4f);
		HideLockedOnUIBoxTimerFinished = false;
	}
}

void AEnemyShip::HideLockedOnUIBox() {
	SetMissleLockOnUIBoxVisibility(false);
	HideLockedOnUIBoxTimerFinished = true;
}

void AEnemyShip::HandleEngineSound() {
	if (PlayEngineSoundTimerFinished) {
		GetWorldTimerManager().ClearTimer(PlayEngineSoundTimer);
		GetWorldTimerManager().SetTimer(PlayEngineSoundTimer, this, &AEnemyShip::PlayEngineSound, 0.5f);
		PlayEngineSoundTimerFinished = false;
	}
}

void AEnemyShip::PlayEngineSound() {
	ShipStatics::PlaySound(EngineSound, this);
	PlayEngineSoundTimerFinished = true;
}

void AEnemyShip::HandleNullingOutAimedAtPlayerShip() {
	if (NullOutAimedAtPlayerShipTimerFinished) {
		GetWorldTimerManager().ClearTimer(NullOutAimedAtPlayerShipTimer);
		GetWorldTimerManager().SetTimer(NullOutAimedAtPlayerShipTimer, this, &AEnemyShip::NullOutAimedAtPlayerShip, 0.3f);
		NullOutAimedAtPlayerShipTimerFinished = false;
	}
}

void AEnemyShip::NullOutAimedAtPlayerShip() {
	AimedAtPlayerShip = nullptr;
	NullOutAimedAtPlayerShipTimerFinished = true;
}

void AEnemyShip::HandleFiringBlasterShots() {
	if (BlasterShotReloadTimerFinished) {
		GetWorldTimerManager().ClearTimer(BlasterShotReloadTimer);
		GetWorldTimerManager().SetTimer(BlasterShotReloadTimer, this, &AEnemyShip::FireBlasterShot, 0.2f);
		BlasterShotReloadTimerFinished = false;
	}
}

void AEnemyShip::FireBlasterShot() {
	if (AimedAtPlayerShip) {
		if (const TObjectPtr<UWorld> World = GetWorld()) {
			const TObjectPtr<UArrowComponent> BarrelToFireFrom = DeterminWhichBarrelToFireFrom();
			TObjectPtr<ABlasterShot> BlasterShot = World->SpawnActor<ABlasterShot>(
				BlasterShotBlueprintClass,
				BarrelToFireFrom->GetComponentLocation(),
				BarrelToFireFrom->GetComponentRotation()
			);
			BlasterShot->FireInDirection(BarrelToFireFrom->GetComponentRotation().Vector());
		}
	}
	BlasterShotReloadTimerFinished = true;
}

TObjectPtr<UArrowComponent> AEnemyShip::DeterminWhichBarrelToFireFrom() {
	if (RightBarrelHasFired) {
		RightBarrelHasFired = false;
		return LeftGunBarrel;
	} else {
		RightBarrelHasFired = true;
		return RightGunBarrel;
	}
}

void AEnemyShip::SetMissleLockOnUIBoxVisibility(const bool& Value) {
	if (MissleLockOnUIBox) MissleLockOnUIBox->SetVisibility(Value);
}
