#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "PatrolTargets/PatrolTarget.h"
#include "Projectiles/BlasterShot.h"
#include "Statics/ShipStatics.h"

AEnemyShip::AEnemyShip() {
	PrimaryActorTick.bCanEverTick = true;
	DetectedPlayerShip = nullptr;
	DetectedPlayerShipNullOutTimerFinished = true;
	TurnSpeed = 0.7f;
	NewPatrolTargetIndex = 0;
	CurrentPatrolTargetIndex = 0;
	Health = 500;

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

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement Component"));
	PawnMovementComponent->MaxSpeed = 5000.0f;

	FieldSystemSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Field System Spawn Location"));
	FieldSystemSpawnLocation->SetupAttachment(GetRootComponent());
}

void AEnemyShip::BeginPlay() {
	Super::BeginPlay();
	SetupPlayerShipDetection();
	SetupTakingHitsFunctionality();
}

void AEnemyShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleChasingRotation();
	AddMovementInput(GetActorForwardVector(), 1.0f);
	HandleDetectedPlayerShipNullOutTimer();
	HandleExploding();
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
}

void AEnemyShip::SetDetectedPlayerShip(APawn* SeenPawn) {
	if (SeenPawn) {
		DetectedPlayerShip = SeenPawn;
	}
}

void AEnemyShip::SetupTakingHitsFunctionality() {
	if (ShipMesh) ShipMesh->OnComponentHit.AddDynamic(this, &AEnemyShip::TakeHit);
}

void AEnemyShip::TakeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (const TObjectPtr<ABlasterShot> BlasterShot = Cast<ABlasterShot>(OtherActor)) {
		TakeBlasterShotHit(BlasterShot);
	}
}

void AEnemyShip::TakeBlasterShotHit(const TObjectPtr<ABlasterShot> BlasterShot) {
	BlasterShot->SpawnImpactBurst();
	Health -= BlasterShot->Damage;
	BlasterShot->Destroy();
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
