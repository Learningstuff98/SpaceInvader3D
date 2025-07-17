#include "GameStates/SpaceInvaderGameState.h"
#include "Components/SphereComponent.h"
#include "PatrolTargets/PatrolTarget.h"
#include "Development/Development.h"
#include "Enemies/EnemyShipSpawnPoint.h"
#include "Enemies/EnemyShip.h"

ASpaceInvaderGameState::ASpaceInvaderGameState() {
	PrimaryActorTick.bCanEverTick = true;
	EnemyShipCount = 0;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	DetectionSphere->SetupAttachment(GetRootComponent());
}

void ASpaceInvaderGameState::BeginPlay() {
	Super::BeginPlay();
	SetupDetectionSphereOverlapFunctionality();
}

void ASpaceInvaderGameState::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleDetectionSphere();
	HandleSpawningEnemyShips();
}

void ASpaceInvaderGameState::SetupDetectionSphereOverlapFunctionality() {
	if (DetectionSphere) {
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpaceInvaderGameState::HandleDetectionSphereOverlaps);
	}
}

void ASpaceInvaderGameState::HandleDetectionSphereOverlaps(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		if (const TObjectPtr<APatrolTarget> PatrolTarget = Cast<APatrolTarget>(OtherActor)) {
			PatrolTargets.Add(PatrolTarget);
		}
		if (const TObjectPtr<AEnemyShipSpawnPoint> EnemyShipSpawnPoint = Cast<AEnemyShipSpawnPoint>(OtherActor)) {
			EnemyShipSpawnPoints.Add(EnemyShipSpawnPoint);
		}
	}
}

void ASpaceInvaderGameState::HandleDetectionSphere() {
	if (DetectionSphere) {
		PerformDetectionSpherePing();
		HandleDiscardingDetectionSphere();
	}
}

void ASpaceInvaderGameState::PerformDetectionSpherePing() {
	DetectionSphere->SetSphereRadius(
		DetectionSphere->GetUnscaledSphereRadius() + 100000.f
	);
}

void ASpaceInvaderGameState::HandleDiscardingDetectionSphere() {
	if (DetectionSphere->GetUnscaledSphereRadius() >= 500000.f) {
		DetectionSphere->DestroyComponent();
		DetectionSphere = nullptr;
	}
}

void ASpaceInvaderGameState::HandleSpawningEnemyShips() {
	if (EnemyShipCount < 3 && EnemyShipSpawnPoints.Num() >= 3) {
		if (const TObjectPtr<UWorld> World = GetWorld()) {
			if (const TObjectPtr<AEnemyShipSpawnPoint> EnemyShipSpawnPoint = GetRandomSpawnPoint()) {
				const TObjectPtr<AEnemyShip> EnemyShip = World->SpawnActor<AEnemyShip>(
					EnemyShipBlueprintClass,
					EnemyShipSpawnPoint->GetActorLocation(),
					EnemyShipSpawnPoint->GetActorRotation()
				);
				EnemyShip->SpawnDefaultController();
			}
		}
	}
}

TObjectPtr<AEnemyShipSpawnPoint> ASpaceInvaderGameState::GetRandomSpawnPoint() {
	return EnemyShipSpawnPoints[FMath::RandRange(0, EnemyShipSpawnPoints.Num() - 1)];
}
