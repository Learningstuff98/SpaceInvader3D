#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipPieces.generated.h"

UCLASS()
class SPACEINVADER3D_API AShipPieces : public AActor
{
	GENERATED_BODY()
	
public:	
	AShipPieces();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGeometryCollectionComponent> Pieces;
};
