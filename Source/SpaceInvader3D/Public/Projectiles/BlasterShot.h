// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlasterShot.generated.h"

UCLASS()
class SPACEINVADER3D_API ABlasterShot : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlasterShot();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
