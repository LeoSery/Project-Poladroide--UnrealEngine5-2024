// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/GameModeInterface.h"
#include "GameModeDroid.generated.h"

/**
 * 
 */
UCLASS()
class POLADROID_API AGameModeDroid : public AGameModeBase , public IGameModeInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable , Category = "GameModeInterface")
	void Start_Lose(AActor* Caller);
	void Start_Lose_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable , Category = "GameModeInterface")
	void Start_Win(AActor* Caller);
	void Start_Win_Implementation(AActor* Caller);
	
};
