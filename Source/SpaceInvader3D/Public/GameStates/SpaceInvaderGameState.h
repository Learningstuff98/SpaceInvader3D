#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceInvaderGameState.generated.h"

UCLASS()
class SPACEINVADER3D_API ASpaceInvaderGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASpaceInvaderGameState();

	//<AActor>
	virtual void Tick(float DeltaTime) override;
	//</AActor>
	
private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Delegate callbacks

	UFUNCTION()
	void HandleDetectionSphereOverlaps(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Functions

	void SetupDetectionSphereOverlapFunctionality();
	void HandleDetectionSphere();
	void PerformDetectionSpherePing();
	void HandleDiscardingDetectionSphere();
	void HandleSpawningEnemyShips();
	TObjectPtr<class AEnemyShipSpawnPoint> GetRandomSpawnPoint();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> DetectionSphere;

	// Other variables

	UPROPERTY(EditAnywhere, Category = "Patrol Targets")
	TArray<TObjectPtr<class APatrolTarget>> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "Spawn Points")
	TArray<TObjectPtr<class AEnemyShipSpawnPoint>> EnemyShipSpawnPoints;

	UPROPERTY(EditAnywhere, Category = Enemies)
	TSubclassOf<class AEnemyShip> EnemyShipBlueprintClass;

	UPROPERTY()
	int32 EnemyShipCount;

public:
	// Setters and getters
	FORCEINLINE void SetEnemyShipCount(const int32& NewCount) { EnemyShipCount = NewCount; }
	FORCEINLINE int32 GetEnemyShipCount() const { return EnemyShipCount; }
};
