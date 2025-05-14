#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyShip.generated.h"

UCLASS()
class SPACEINVADER3D_API AEnemyShip : public APawn
{
	GENERATED_BODY()
	
public:	
	AEnemyShip();

	//<AActor>
	virtual void Tick(float DeltaTime) override;
	//</AActor>

private:
	// <APawn>
	virtual void BeginPlay() override;
	// </APawn>

	// Dynamic multicast delegate callbacks

	UFUNCTION()
	void PlayerShipDetected(APawn* SeenPawn);

	// Miscellaneous
	void MoveTowardsTarget();
	void HandleUpdatingDestination();
	void SetupPlayerShipDetection();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> ShipMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> InitialFollowTarget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;

	// Other

	UPROPERTY()
	FVector Destination;
};
