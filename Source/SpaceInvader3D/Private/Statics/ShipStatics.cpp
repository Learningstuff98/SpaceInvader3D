#include "Statics/ShipStatics.h"
#include "ExplodingEffects/ShipExplodingEffect.h"
#include "ShipPieces/ShipPieces.h"
#include "Field/FieldSystemActor.h"
#include "Kismet/GameplayStatics.h"

void ShipStatics::SpawnShipExplodingEffect(const TSubclassOf<AShipExplodingEffect> ShipExplodingEffectBlueprintClass, const TObjectPtr<AActor> Ship) {
	if (const TObjectPtr<UWorld> World = Ship->GetWorld()) {
		World->SpawnActor<AShipExplodingEffect>(
			ShipExplodingEffectBlueprintClass,
			Ship->GetActorLocation(),
			Ship->GetActorRotation()
		);
	}
}

void ShipStatics::SpawnShipPieces(const TSubclassOf<class AShipPieces> ShipPiecesBlueprintClass, const TObjectPtr<AActor> Ship) {
	if (const TObjectPtr<UWorld> World = Ship->GetWorld()) {
		World->SpawnActor<AShipPieces>(
			ShipPiecesBlueprintClass,
			Ship->GetActorLocation(),
			Ship->GetActorRotation()
		);
	}
}

void ShipStatics::SpawnShipExplodingFieldSystem(const TSubclassOf<AFieldSystemActor> ShipExplodingFieldSystemBlueprintClass, const TObjectPtr<AActor> Ship) {
	if (const TObjectPtr<UWorld> World = Ship->GetWorld()) {
		World->SpawnActor<AFieldSystemActor>(
			ShipExplodingFieldSystemBlueprintClass,
			Ship->GetActorLocation(),
			Ship->GetActorRotation()
		);
	}
}

void ShipStatics::PlayExplodingSound(const TObjectPtr<USoundBase> ExplodingSound, const TObjectPtr<AActor> Ship) {
	UGameplayStatics::PlaySoundAtLocation(
		Ship,
		ExplodingSound,
		Ship->GetActorLocation()
	);
}
