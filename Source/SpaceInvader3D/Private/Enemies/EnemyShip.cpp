#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "PatrolTargets/PatrolTarget.h"

AEnemyShip::AEnemyShip() {
	PrimaryActorTick.bCanEverTick = true;
	DetectedPlayerShip = nullptr;
	bDetectedPlayerShipNullOutTimerFinished = true;
	TurnSpeed = 0.5f;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	SetRootComponent(ShipMesh);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SetPeripheralVisionAngle(70.0f);

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement Component"));
	PawnMovementComponent->MaxSpeed = 5000.0f;
}

void AEnemyShip::BeginPlay() {
	Super::BeginPlay();
	SetupPlayerShipDetection();
}

void AEnemyShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleChasingRotation();
	AddMovementInput(GetActorForwardVector(), 1.0f);
	HandleDetectedPlayerShipNullOutTimer();
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
		PatrolTargets[0]->GetActorLocation()
	);
}

FRotator AEnemyShip::GetPlayerShipLookAtRotation() {
	return UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		DetectedPlayerShip->GetActorLocation()
	);
}

void AEnemyShip::HandleDetectedPlayerShipNullOutTimer() {
	if (bDetectedPlayerShipNullOutTimerFinished) {
		GetWorldTimerManager().ClearTimer(DetectedPlayerShipNullOutTimer);
		GetWorldTimerManager().SetTimer(DetectedPlayerShipNullOutTimer, this, &AEnemyShip::NullOutDetectedPlayerShip, 0.1f);
		bDetectedPlayerShipNullOutTimerFinished = false;
	}
}

void AEnemyShip::NullOutDetectedPlayerShip() {
	DetectedPlayerShip = nullptr;
	bDetectedPlayerShipNullOutTimerFinished = true;
}

void AEnemyShip::SetupPlayerShipDetection() {
	if (PawnSensingComponent) {
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyShip::PlayerShipDetected);
	}
}

void AEnemyShip::PlayerShipDetected(APawn* SeenPawn) {
	if (SeenPawn) {
		DetectedPlayerShip = SeenPawn;
	}
}
