#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missle.generated.h"

UCLASS()
class SPACEINVADER3D_API AMissle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissle();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:

	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>
};
