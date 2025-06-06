#pragma once

#include "CoreMinimal.h"

class SPACEINVADER3D_API ShipStatics
{
public:
	static void SpawnShipExplodingEffect(const TSubclassOf<class AShipExplodingEffect> ShipExplodingEffectBlueprintClass, const TObjectPtr<AActor> Ship);
	static void SpawnShipPieces(const TSubclassOf<class AShipPieces> ShipPiecesBlueprintClass, const TObjectPtr<AActor> Ship);
	static void SpawnShipExplodingFieldSystem(const TSubclassOf<class AFieldSystemActor> ShipExplodingFieldSystemBlueprintClass, const TObjectPtr<UWorld> World, const FVector& FieldSystemSpawnLocation, const FRotator& FieldSystemSpawnRotation);
	static void PlaySound(const TObjectPtr<USoundBase> Sound, const TObjectPtr<AActor> Ship);
};
