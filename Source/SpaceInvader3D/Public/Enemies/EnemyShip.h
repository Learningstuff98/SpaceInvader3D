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

	// Dynamic Multicast Delegate Callbacks

	UFUNCTION()
	void PlayerShipDetected(APawn* SeenPawn);

	// Miscellaneous
	void SetupPlayerShipDetection();
	void HandleChasingRotation();
	FRotator GetChasingRotation(const FRotator& LookAtRotation);
	FRotator GetPlayerShipLookAtRotation();
	FRotator GetPatrolTargetLookAtRotation();
	void HandleDetectedPlayerShipNullOutTimer();
	void NullOutDetectedPlayerShip();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> ShipMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFloatingPawnMovement> PawnMovementComponent;

	// Other

	UPROPERTY()
	APawn* DetectedPlayerShip;

	FTimerHandle DetectedPlayerShipNullOutTimer;

	UPROPERTY()
	bool bDetectedPlayerShipNullOutTimerFinished;

	UPROPERTY(EditAnywhere, Category = Patrolling)
	TArray<TObjectPtr<class APatrolTarget>> PatrolTargets;

	UPROPERTY()
	float TurnSpeed;
};
