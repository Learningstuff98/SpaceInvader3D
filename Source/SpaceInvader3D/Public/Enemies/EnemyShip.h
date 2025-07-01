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
	void TakeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Functions

	void SetupPlayerShipDetection();
	void HandleChasingRotation();
	FRotator GetChasingRotation(const FRotator& LookAtRotation);
	FRotator GetPlayerShipLookAtRotation();
	FRotator GetPatrolTargetLookAtRotation();
	void HandleDetectedPlayerShipNullOutTimer();
	void NullOutDetectedPlayerShip();
	void TakeBlasterShotHit(const TObjectPtr<class ABlasterShot> BlasterShot);
	void HandleExploding();
	void Explode();
	void SetupTakingHitsFunctionality();
	void UpdateMissleLockOnUIBoxRotation();
	void HandleHidingLockedOnUIBox();
	void HideLockedOnUIBox();
	void HandleEngineSound();
	void PlayEngineSound();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> ShipMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFloatingPawnMovement> PawnMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> FieldSystemSpawnLocation;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> MissleLockOnUIBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> EngineThrusterEffect;

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

public:
	// Setters
	FORCEINLINE void SetPlayerShip(const TObjectPtr<class APlayerShip> Ship) { PlayerShip = Ship; }
	void SetMissleLockOnUIBoxVisibility(const bool& Value);
	FORCEINLINE void TakeDamage(const int32& Damage) { Health -= Damage; }
};
