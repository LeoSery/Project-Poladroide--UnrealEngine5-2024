// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CheckpointSubsystem.generated.h"

/**
 * 
 */
USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	/* Class of the Actor */
	UPROPERTY()
	UClass* Class;

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY()
	FTransform Transform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class POLADROID_API UCheckpointSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Respawn" , meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void RespawnPlayerAtLastCheckpoint(const UObject* WorldContextObject , APlayerRobot* PlayerRobot);

	UFUNCTION(BlueprintCallable, Category = "Respawn" , meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void SavePlayerCheckpoint(const UObject* WorldContextObject,FTransform CheckpointTransform);

	
private:
	/* Get all the data needed to save the Actor */
	FActorSaveData GetActorSaveData(AActor* ActorToSave);

	UFUNCTION()
	void Internal_SavePlayerCheckpoint(const FTransform& CheckpointTransform);

protected:
	FTransform LastCheckpointTransform;
	
};
