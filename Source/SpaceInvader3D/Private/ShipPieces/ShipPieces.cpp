#include "ShipPieces/ShipPieces.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AShipPieces::AShipPieces() {
	PrimaryActorTick.bCanEverTick = true;

	Pieces = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Pieces"));
	SetRootComponent(Pieces);
}

void AShipPieces::BeginPlay() {
	Super::BeginPlay();
}

void AShipPieces::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
