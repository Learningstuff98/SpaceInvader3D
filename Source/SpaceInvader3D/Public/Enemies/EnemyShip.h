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

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> FieldSystemSpawnLocation;

	// Miscellaneous

	void GetRandomPatrolTargetIndex();

private:
	// <APawn>
	virtual void BeginPlay() override;
	// </APawn>

	// Dynamic Multicast Delegate Callbacks

	UFUNCTION()
	void SetDetectedPlayerShip(APawn* SeenPawn);

	UFUNCTION()
	void TakeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Miscellaneous
	void SetupPlayerShipDetection();
	void HandleChasingRotation();
	FRotator GetChasingRotation(const FRotator& LookAtRotation);
	FRotator GetPlayerShipLookAtRotation();
	FRotator GetPatrolTargetLookAtRotation();
	void HandleDetectedPlayerShipNullOutTimer();
	void NullOutDetectedPlayerShip();
	void TakeBlasterShotHit(const TObjectPtr<class ABlasterShot> BlasterShot);
	void HandleBlowingUp();
	void SetupTakingHitsFunctionality();

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
	int32 NewPatrolTargetIndex;

	UPROPERTY()
	int32 CurrentPatrolTargetIndex;

	UPROPERTY()
	float TurnSpeed;

	UPROPERTY()
	int32 Health;

	UPROPERTY(EditAnywhere, Category = Pieces)
	TSubclassOf<class AShipPieces> ShipPiecesBlueprintClass;

	UPROPERTY(EditAnywhere, Category = "Field Systems")
	TSubclassOf<class AFieldSystemActor> ShipExplodingFieldSystemBlueprintClass;

	UPROPERTY(EditAnywhere, Category = "Explosion Effect")
	TSubclassOf<class AShipExplodingEffect> ShipExplodingEffectBlueprintClass;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> ExplodingSound;
};
