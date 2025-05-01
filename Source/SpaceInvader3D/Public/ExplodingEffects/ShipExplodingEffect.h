#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipExplodingEffect.generated.h"

UCLASS()
class SPACEINVADER3D_API AShipExplodingEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AShipExplodingEffect();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> ExplosionEffect;
};
