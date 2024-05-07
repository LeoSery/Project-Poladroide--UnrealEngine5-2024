// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/WidgetInterface.h"
#include "WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class POLADROID_API UWidgetBase : public UUserWidget ,public IWidgetInterface
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void FocusWidget(bool bFocus);
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Widget" , meta = (ExposeOnSpawn=true))
	UUserWidget* ParentWidget;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetParentWidget(UUserWidget* InParentWidget);

};
