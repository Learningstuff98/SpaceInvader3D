// Fill out your copyright notice in the Description page of Project Settings.


#include "Development/Development.h"

void Development::LogMessage(const FString& Message) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}

void Development::LogFloat(const float& Float) {
	FString Message = FString::Printf(TEXT("THE FLOAT VALUE IS: %f"), Float);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}

void Development::LogDouble(const double& Double) {
	FString Message = FString::Printf(TEXT("THE DOUBLE VALUE IS: %f"), Double);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}
