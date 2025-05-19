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
		SetActorRotation(GetNewChasingRotation(0.5f));
	} else if (PatrolTargets.Num() > 0) {
		SetActorRotation(
			UKismetMathLibrary::RInterpTo(
				GetActorRotation(),
				UKismetMathLibrary::FindLookAtRotation(
					GetActorLocation(),
					PatrolTargets[0]->GetActorLocation()
				),
				UGameplayStatics::GetWorldDeltaSeconds(this),
				0.5f
			)
		);
	}
}

FRotator AEnemyShip::GetNewChasingRotation(const float& InterpSpeed) {
	return UKismetMathLibrary::RInterpTo(
		GetActorRotation(),
		GetLookAtRotation(),
		UGameplayStatics::GetWorldDeltaSeconds(this),
		InterpSpeed
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

FRotator AEnemyShip::GetLookAtRotation() {
	return UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		DetectedPlayerShip->GetActorLocation()
	);
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
