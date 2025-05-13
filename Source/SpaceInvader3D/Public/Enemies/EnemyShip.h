#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyShip.generated.h"

UCLASS()
class SPACEINVADER3D_API AEnemyShip : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyShip();

	//<AActor>
	virtual void Tick(float DeltaTime) override;
	//</AActor>

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Miscellaneous
	void MoveTowardsTarget();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> ShipMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> InitialFollowTarget;
};
