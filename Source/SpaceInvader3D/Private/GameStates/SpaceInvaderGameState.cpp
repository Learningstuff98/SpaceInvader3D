#include "GameStates/SpaceInvaderGameState.h"
#include "Components/SphereComponent.h"
#include "PatrolTargets/PatrolTarget.h"
#include "Development/Development.h"
#include "Enemies/EnemyShipSpawnPoint.h"

ASpaceInvaderGameState::ASpaceInvaderGameState() {
	PrimaryActorTick.bCanEverTick = true;

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
			Development::LogMessage("SPAWN POINT WAS FOUND");
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
