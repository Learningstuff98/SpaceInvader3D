#include "PatrolTargets/PatrolTarget.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Enemies/EnemyShip.h"

#include "Development/Development.h"

APatrolTarget::APatrolTarget() {
	PrimaryActorTick.bCanEverTick = true;

	EnemyShipDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Ship Detection Sphere"));
	SetRootComponent(EnemyShipDetectionSphere);
	EnemyShipDetectionSphere->SetSphereRadius(2000.0f);
}

void APatrolTarget::BeginPlay() {
	Super::BeginPlay();
	SetupSphereInteractionsFunctionality();
}

void APatrolTarget::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APatrolTarget::TellEnemyShiptoFindANewPatrolTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(OtherActor)) {
		EnemyShip->GetRandomPatrolTargetIndex();
		Development::LogMessage("End Overlap was called");
	}
}

void APatrolTarget::SetupSphereInteractionsFunctionality() {
	if (EnemyShipDetectionSphere) {
		EnemyShipDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &APatrolTarget::TellEnemyShiptoFindANewPatrolTarget);
	}
}
