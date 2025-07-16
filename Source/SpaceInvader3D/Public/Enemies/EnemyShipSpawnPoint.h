#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyShipSpawnPoint.generated.h"

UCLASS()
class SPACEINVADER3D_API AEnemyShipSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyShipSpawnPoint();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:

	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> Sphere;
};
