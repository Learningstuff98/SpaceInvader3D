#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissileExplodingEffect.generated.h"

UCLASS()
class SPACEINVADER3D_API AMissileExplodingEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissileExplodingEffect();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:

	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	 // Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> ExplosionEffect;
};
