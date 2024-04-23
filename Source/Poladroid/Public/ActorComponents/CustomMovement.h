// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovement.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_WallRun		UMETA(DisplayName = "Wall Run"),
	CMOVE_WallWalk		UMETA(DisplayName = "Wall Run"),
	CMOVE_Hang			UMETA(DisplayName = "Hang"),
	CMOVE_Climb			UMETA(DisplayName = "Climb"),
	CMOVE_MAX			UMETA(Hidden),
};
/**
 * 
 */

UCLASS()
class POLADROID_API UCustomMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool first = true;
	bool CameraMovement = false;
	bool InAngle = false;
	FRotator dest = {0,0,0};
	FTransform lastWallPosition;

	TArray<FVector> LastWallPosition[4];
	int index = 0;
	
#pragma region WallWalk

	void PhysWallWalking(float deltaTime, int32 Iterations);
	
	bool CanWalkOnWall();
	
#pragma endregion
	
#pragma region Wallrun
public:
	UPROPERTY(Category="WallRun", EditAnywhere, BlueprintReadWrite)
	FVector2f WallRun_Scale = FVector2f(1,1); // X = Radius , Y = Height
	UPROPERTY(Category="WallRun", EditAnywhere, BlueprintReadWrite)
	float WallRun_MaxAngle = 0.5f; // Max angle to wallrun
	UPROPERTY(Category="WallRun", EditAnywhere, BlueprintReadWrite)
	float WallRun_MinAngle = 0.5f; // Min angle to wallrun
	UPROPERTY(Category="WallRun", EditAnywhere, BlueprintReadWrite)
	int WallRun_FramePrediction; // Number of frames predicted when looking for wall
	UPROPERTY(Category="WallRun", EditAnywhere, BlueprintReadWrite)
	int Wall_RunMaxSpeed = 1000.f;
	
	bool bWantToJumpFromWall = false;

	/*Return true if Player want to wallrun and CanWallRun()*/
	bool WantToWallRun();
	bool CanWallRun();
	//Return true if wall can be runned on
	bool IsWallRunnable(FHitResult Hit);
	
	bool IsHittingWall(FHitResult& Hit);

	void PhysWallRun(float deltaTime, int32 Iterations);

	FVector GetWallRunDirection(FHitResult Hit);
	FVector CalcWallrunVelocity(float timeTick , FVector WallRunDirection);
#pragma endregion

#pragma region CollisionTest

public:
	float GetCapsuleRadius();
	float GetCapsuleHalfHeight();
	FHitResult GetFutureCollision(float DeltaSeconds); //Future collision based on current velocity and deltatime
	FHitResult FindBaseWall();

protected:
	FHitResult LastHitTest;
#pragma endregion 
};
