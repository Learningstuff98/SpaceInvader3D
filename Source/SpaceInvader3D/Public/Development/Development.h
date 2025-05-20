#pragma once

#include "CoreMinimal.h"

class SPACEINVADER3D_API Development
{
public:
	static void LogMessage(const FString& Message);
	static void LogNumber(const float& Float);
	static void LogNumber(const double& Double);
	static void LogNumber(const int32& Integer);
};
