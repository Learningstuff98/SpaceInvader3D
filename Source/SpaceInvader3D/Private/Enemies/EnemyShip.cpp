#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"

AEnemyShip::AEnemyShip() {
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	SetRootComponent(ShipMesh);

	InitialFollowTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Initial Follow Target"));
	InitialFollowTarget->SetupAttachment(GetRootComponent());
}

void AEnemyShip::BeginPlay() {
	Super::BeginPlay();
}

void AEnemyShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	MoveTowardsTarget();
}

void AEnemyShip::MoveTowardsTarget() {
	if (InitialFollowTarget) {
		const FVector CurrentLocation = FMath::VInterpTo(
			GetActorLocation(),
			InitialFollowTarget->GetComponentLocation(),
			UGameplayStatics::GetWorldDeltaSeconds(this),
			6.0f
		);
		SetActorLocation(CurrentLocation);
	}
}
