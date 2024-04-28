// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/World/CheckpointSubsystem.h"

#include "Actor/Character/PlayerRobot.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void UCheckpointSubsystem::RespawnPlayerAtLastCheckpoint(const UObject* WorldContextObject , APlayerRobot* PlayerRobot)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr)
	{
		const FActorSaveData ActorData =  World->GetSubsystem<UCheckpointSubsystem>()->GetActorSaveData(PlayerRobot);

		AController* PlayerController = PlayerRobot->GetController();
		
		PlayerRobot->Destroy();

		//Spawn the Actor
		AActor* NewActor = World->SpawnActor(ActorData.Class, &ActorData.Transform);
		//Restore the Actor's state
		FMemoryReader MemReader(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		// Convert binary array back into actor's variables
		NewActor->Serialize(Ar);
		PlayerController->Possess(Cast<APawn>(NewActor));
	}
}

void UCheckpointSubsystem::SavePlayerCheckpoint(const UObject* WorldContextObject, FTransform CheckpointTransform)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr)
	{
		World->GetSubsystem<UCheckpointSubsystem>()->Internal_SavePlayerCheckpoint(CheckpointTransform);
	}
}

FActorSaveData UCheckpointSubsystem::GetActorSaveData(AActor* ActorToSave)
{
	FActorSaveData ActorData;
	ActorData.Class = ActorToSave->GetClass();
	ActorData.Transform = this->LastCheckpointTransform;
	
	// Pass the array to fill with data from Actor
	FMemoryWriter MemWriter(ActorData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	// Find only variables with UPROPERTY(SaveGame)
	Ar.ArIsSaveGame = true;
	// Converts Actor's SaveGame UPROPERTIES into binary array
	ActorToSave->Serialize(Ar);
	
	return ActorData;
}

void UCheckpointSubsystem::Internal_SavePlayerCheckpoint(const FTransform& CheckpointTransform)
{
	LastCheckpointTransform = CheckpointTransform;
}
