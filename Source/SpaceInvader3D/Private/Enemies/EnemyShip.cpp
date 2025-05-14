#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Development/Development.h"

AEnemyShip::AEnemyShip() {
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	SetRootComponent(ShipMesh);

	InitialFollowTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Initial Follow Target"));
	InitialFollowTarget->SetupAttachment(GetRootComponent());

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
}

void AEnemyShip::BeginPlay() {
	Super::BeginPlay();
	SetupPlayerShipDetection();
}

void AEnemyShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	MoveTowardsTarget();
	HandleUpdatingDestination();
}

void AEnemyShip::SetupPlayerShipDetection() {
	if (PawnSensingComponent) {
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyShip::PlayerShipDetected);
	}
}

void AEnemyShip::PlayerShipDetected(APawn* SeenPawn) {
	Development::LogMessage("THE PLAYERSHIP WAS DETECTED");
}

void AEnemyShip::HandleUpdatingDestination() {
	if (InitialFollowTarget) {
		Destination = InitialFollowTarget->GetComponentLocation();
	}
}

void AEnemyShip::MoveTowardsTarget() {
	if (InitialFollowTarget) {
		const FVector CurrentLocation = FMath::VInterpTo(
			GetActorLocation(),
			Destination,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			2.5f
		);
		SetActorLocation(CurrentLocation);
	}
}
