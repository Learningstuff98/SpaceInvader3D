#include "Statics/ShipStatics.h"
#include "ExplodingEffects/ShipExplodingEffect.h"
#include "ShipPieces/ShipPieces.h"
#include "Field/FieldSystemActor.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/EnemyShip.h"
#include "PlayerShip/PlayerShip.h"

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
		if (const TObjectPtr<APlayerShip> PlayerShip = Cast<APlayerShip>(Ship)) {
			SpawnPlayerShipFieldSystem(PlayerShip, World, ShipExplodingFieldSystemBlueprintClass);
		}
		if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(Ship)) {
			SpawnEnemyShipFieldSystem(EnemyShip, World, ShipExplodingFieldSystemBlueprintClass);
		}
	}
}

void ShipStatics::PlayExplodingSound(const TObjectPtr<USoundBase> ExplodingSound, const TObjectPtr<AActor> Ship) {
	UGameplayStatics::PlaySoundAtLocation(
		Ship,
		ExplodingSound,
		Ship->GetActorLocation()
	);
}

void ShipStatics::SpawnPlayerShipFieldSystem(const TObjectPtr<APlayerShip> PlayerShip, const TObjectPtr<UWorld> World, const TSubclassOf<AFieldSystemActor> ShipExplodingFieldSystemBlueprintClass) {
	World->SpawnActor<AFieldSystemActor>(
		ShipExplodingFieldSystemBlueprintClass,
		PlayerShip->FieldSystemSpawnLocation->GetComponentLocation(),
		PlayerShip->GetActorRotation()
	);
}

void ShipStatics::SpawnEnemyShipFieldSystem(const TObjectPtr<AEnemyShip> EnemyShip, const TObjectPtr<UWorld> World, const TSubclassOf<AFieldSystemActor> ShipExplodingFieldSystemBlueprintClass) {
	World->SpawnActor<AFieldSystemActor>(
		ShipExplodingFieldSystemBlueprintClass,
		EnemyShip->FieldSystemSpawnLocation->GetComponentLocation(),
		EnemyShip->GetActorRotation()
	);
}
