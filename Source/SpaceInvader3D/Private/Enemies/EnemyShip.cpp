#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyShip::AEnemyShip() {
	PrimaryActorTick.bCanEverTick = true;
	DetectedPlayerShip = nullptr;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	SetRootComponent(ShipMesh);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement Component"));
	PawnMovementComponent->MaxSpeed = 4000.0f;
}

void AEnemyShip::BeginPlay() {
	Super::BeginPlay();
	SetupPlayerShipDetection();
}

void AEnemyShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleChasingRotation();
	AddMovementInput(GetActorForwardVector(), 1.0f);
}

void AEnemyShip::HandleChasingRotation() {
	if (DetectedPlayerShip) {
		SetActorRotation(
			UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(),
				DetectedPlayerShip->GetActorLocation()
			)
		);
	}
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
