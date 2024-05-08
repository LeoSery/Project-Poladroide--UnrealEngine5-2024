// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAppearEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisappearEnd);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class POLADROID_API IWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void MakeWidgetDisappear(bool bInstantaneous);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void MakeWidgetAppear(bool bInstantaneous);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void Return();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void Interact(uint8 EnumValue);
	
};
