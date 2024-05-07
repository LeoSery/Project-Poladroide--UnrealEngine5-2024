// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetBase.h"

#include "Kismet/GameplayStatics.h"

void UWidgetBase::FocusWidget(bool bFocus)
{
	UGameplayStatics::SetGamePaused(GetWorld(), bFocus);
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{	
		if (bFocus)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
			FInputModeGameAndUI InputMode;
			PlayerController->SetInputMode(InputMode);
		}
		else
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}
	}
}

void UWidgetBase::SetParentWidget(UUserWidget* InParentWidget)
{
	ParentWidget = InParentWidget;
}
