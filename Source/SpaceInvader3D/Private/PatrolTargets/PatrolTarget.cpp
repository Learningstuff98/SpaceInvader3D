#include "PatrolTargets/PatrolTarget.h"
#include "NiagaraComponent.h"

APatrolTarget::APatrolTarget() {
	PrimaryActorTick.bCanEverTick = true;
}

void APatrolTarget::BeginPlay() {
	Super::BeginPlay();
}

void APatrolTarget::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
