// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceLightLevel.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceLightLevel : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class ELightLevel : uint8
{
	LightLevel_None		UMETA(Hidden),
	LightLevel_Low		UMETA(DisplayName = "Low"),
	LightLevel_Medium	UMETA(DisplayName = "Medium"),
	LightLevel_High		UMETA(DisplayName = "High"),
	LightLevel_MAX		UMETA(Hidden),
};

/**
 * 
 */
class POLADROID_API IInterfaceLightLevel
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable , BlueprintNativeEvent , Category = "LightLevel")
	float GetLightLevelValue();
	UFUNCTION(BlueprintCallable , BlueprintNativeEvent , Category = "LightLevel")
	ELightLevel GetLightLevel();
};
