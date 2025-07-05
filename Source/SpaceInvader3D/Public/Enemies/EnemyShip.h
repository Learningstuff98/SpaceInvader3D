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

	// Functions

	void GetRandomPatrolTargetIndex();

private:
	// <APawn>
	virtual void BeginPlay() override;
	// </APawn>

	// Delegate Callbacks

	UFUNCTION()
	void SetDetectedPlayerShip(APawn* SeenPawn);

	UFUNCTION()
	void SetAimedAtPlayerShip(APawn* SeenPawn);

	// Functions

	void SetupPlayerShipDetection();
	void HandleChasingRotation();
	FRotator GetChasingRotation(const FRotator& LookAtRotation);
	FRotator GetPlayerShipLookAtRotation();
	FRotator GetPatrolTargetLookAtRotation();
	void HandleDetectedPlayerShipNullOutTimer();
	void NullOutDetectedPlayerShip();
	void HandleExploding();
	void Explode();
	void UpdateMissleLockOnUIBoxRotation();
	void HandleHidingLockedOnUIBox();
	void HideLockedOnUIBox();
	void HandleEngineSound();
	void PlayEngineSound();
	void HandleNullingOutAimedAtPlayerShip();
	void NullOutAimedAtPlayerShip();
	void HandleFiringBlasterShots();
	void FireBlasterShot();
	TObjectPtr<class UArrowComponent> DeterminWhichBarrelToFireFrom();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> ShipMesh;

	// For spotting the player ship
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;

	// For determining if the enemy ship is aiming close enough to the player ship
	// to start firing
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> AimingSensingComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFloatingPawnMovement> PawnMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> FieldSystemSpawnLocation;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> MissleLockOnUIBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> EngineThrusterEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> LeftGunBarrel;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> RightGunBarrel;

	// Other

	UPROPERTY()
	APawn* DetectedPlayerShip;

	FTimerHandle DetectedPlayerShipNullOutTimer;

	UPROPERTY()
	bool DetectedPlayerShipNullOutTimerFinished;

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

	UPROPERTY()
	TObjectPtr<class APlayerShip> PlayerShip;

	FTimerHandle HideLockedOnUIBoxTimer;

	UPROPERTY()
	bool HideLockedOnUIBoxTimerFinished;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> EngineSound;

	FTimerHandle PlayEngineSoundTimer;

	UPROPERTY();
	bool PlayEngineSoundTimerFinished;

	UPROPERTY()
	APawn* AimedAtPlayerShip;

	FTimerHandle NullOutAimedAtPlayerShipTimer;

	UPROPERTY()
	bool NullOutAimedAtPlayerShipTimerFinished;

	FTimerHandle BlasterShotReloadTimer;

	UPROPERTY()
	bool BlasterShotReloadTimerFinished;

	UPROPERTY(EditAnywhere, Category = Projectiles)
	TSubclassOf<class ABlasterShot> BlasterShotBlueprintClass;

	UPROPERTY()
	bool RightBarrelHasFired;

public:
	// Setters
	FORCEINLINE void SetPlayerShip(const TObjectPtr<class APlayerShip> Ship) { PlayerShip = Ship; }
	void SetMissleLockOnUIBoxVisibility(const bool& Value);
	FORCEINLINE void TakeDamage(const int32& Damage) { Health -= Damage; }
};
