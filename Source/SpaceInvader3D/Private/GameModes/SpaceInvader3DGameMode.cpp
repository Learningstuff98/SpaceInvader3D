#include "GameModes/SpaceInvader3DGameMode.h"
#include "GameStates/SpaceInvaderGameState.h"
#include "Development/Development.h"
#include "Components/SphereComponent.h"
#include "PatrolTargets/PatrolTarget.h"

ASpaceInvader3DGameMode::ASpaceInvader3DGameMode() {
	PrimaryActorTick.bCanEverTick = true;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	DetectionSphere->SetupAttachment(GetRootComponent());
}

void ASpaceInvader3DGameMode::BeginPlay() {
	Super::BeginPlay();
	SetupDetectionSphereOverlapFunctionality();
	SetSpaceInvaderGameState();
}

void ASpaceInvader3DGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	HandleDetectionSphere();
	Development::LogNumber(SpaceInvaderGameState->GetEnemyShipCount());
}

void ASpaceInvader3DGameMode::SetupDetectionSphereOverlapFunctionality() {
	if (DetectionSphere) {
		DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpaceInvader3DGameMode::HandleDetectionSphereOverlaps);
	}
}

void ASpaceInvader3DGameMode::SetSpaceInvaderGameState() {
	if (const TObjectPtr<UWorld> World = GetWorld()) {
		if (const TObjectPtr<ASpaceInvaderGameState> CustomGameState = World->GetGameState<ASpaceInvaderGameState>()) {
			SpaceInvaderGameState = CustomGameState;
		}
	}
}

void ASpaceInvader3DGameMode::HandleDetectionSphereOverlaps(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		if (const TObjectPtr<APatrolTarget> PatrolTarget = Cast<APatrolTarget>(OtherActor)) {
			PatrolTargets.Add(PatrolTarget);
		}
	}
}

void ASpaceInvader3DGameMode::HandleDetectionSphere() {
	if (DetectionSphere) {
		PerformDetectionSpherePing();
		HandleDiscardingDetectionSphere();
	}
}

void ASpaceInvader3DGameMode::PerformDetectionSpherePing() {
	DetectionSphere->SetSphereRadius(
		DetectionSphere->GetUnscaledSphereRadius() + 100000.f
	);
}

void ASpaceInvader3DGameMode::HandleDiscardingDetectionSphere() {
	if (DetectionSphere->GetUnscaledSphereRadius() >= 500000.f) {
		DetectionSphere->DestroyComponent();
		DetectionSphere = nullptr;
	}
}
