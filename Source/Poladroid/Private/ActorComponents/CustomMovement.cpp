// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/CustomMovement.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

bool UCustomMovement::IsMovingOnGround() const
{
	return Super::IsMovingOnGround();
}

bool UCustomMovement::CanCrouchInCurrentState() const
{
	//ajouter les conditions a la suite du Supper comme dans un if() voir IsMovingOnGround()
	return Super::CanCrouchInCurrentState();
}

void UCustomMovement::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
	switch (MovementMode)
	{
		case MOVE_Walking:

			if (GetFutureCollision(DeltaSeconds).bBlockingHit)
			{
				LastHitTest = GetFutureCollision(DeltaSeconds);
				SetMovementMode(MOVE_Custom, CMOVE_WallWalk);
			}

		break;
		case MOVE_Falling:
			if (WantToWallRun())
			{
				SetMovementMode(MOVE_Custom, CMOVE_WallRun);
			}
			break;
		default:
			break;
	}

}

void UCustomMovement::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);
	
	switch (CustomMovementMode)
	{
		case CMOVE_WallRun:
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("WallRun")));
			PhysWallRun(deltaTime, Iterations);
			break;
		
		case CMOVE_WallWalk:
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("WallWalk")));
			PhysWallWalking(deltaTime, Iterations);
			break;
	
		default:
			break;
	}
	
}

void UCustomMovement::PhysWallWalking(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}
	
	//Get Ready for some math my dude
	
	
	
	FVector CameraForward = UKismetMathLibrary::GetForwardVector(CharacterOwner->GetControlRotation());
	
	FVector RotFromZ = UKismetMathLibrary::GetForwardVector( UKismetMathLibrary::MakeRotFromZ(LastHitTest.ImpactNormal)) ;//perpendicular of the normal horizontal
	FVector RotFromY = UKismetMathLibrary::GetForwardVector( UKismetMathLibrary::MakeRotFromY(LastHitTest.ImpactNormal));//perpendicular of the normal vertical
	
	FVector CameraDirection =  (RotFromZ.Dot(CameraForward) * RotFromZ + RotFromY.Dot(CameraForward) * RotFromY).GetSafeNormal(); // direction in witch the player look on the wall
	
	DrawDebugDirectionalArrow(GetWorld(), CharacterOwner->GetActorLocation()
		, CharacterOwner->GetActorLocation() + CameraDirection * 50, 100
		, FColor::Magenta, false
		, UpdatedComponent->GetComponentTickInterval()
		, 1, 1);
	
	FQuat DirectionQuat = CameraDirection.ToOrientationQuat();
	
	float DotDirection = FVector::DotProduct(CameraDirection,RotFromZ);
	
	FVector CameraRightDirection = (DotDirection >= 0) ? DirectionQuat.GetUpVector() : UKismetMathLibrary::RotateAngleAxis(CameraDirection , 90.f,DirectionQuat.GetRightVector()) ; // la on fait le right vector de direction UwU
	
	DrawDebugDirectionalArrow(GetWorld(), CharacterOwner->GetActorLocation()
		, CharacterOwner->GetActorLocation() + CameraRightDirection * 100, 100
		, FColor::Blue, false
		, UpdatedComponent->GetComponentTickInterval()
		, 1, 1);
	
	//FVector2D InputDirection =  FVector2D(GetLastInputVector().Dot(CharacterOwner->GetActorForwardVector()) , GetLastInputVector().Dot(CharacterOwner->GetActorRightVector()));
	FRotator WallNormalRotation = UKismetMathLibrary::MakeRotFromZ(LastHitTest.ImpactNormal);
	FRotator NewRotation = UKismetMathLibrary::MakeRotFromZX(LastHitTest.ImpactNormal, CameraDirection);

	FVector UnrotateVector = UKismetMathLibrary::LessLess_VectorRotator(GetLastInputVector(), NewRotation);
	FVector2D InputDirection =  FVector2D(UnrotateVector.X, UnrotateVector.Y);
	
	

	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Input²Direction : %s"), *UnrotateVector.ToString()));

	FVector Direction = CameraDirection * InputDirection.X + CameraRightDirection*InputDirection.Y  ; // la on fait la direction du joueur sur le mur en fonction de son input
	
	DrawDebugDirectionalArrow(GetWorld(), CharacterOwner->GetActorLocation()
		, CharacterOwner->GetActorLocation() + Direction * 100, 100
		, FColor::Green, false
		, UpdatedComponent->GetComponentTickInterval()
		, 1, 1);
	
	Velocity = Direction.GetSafeNormal() * MaxWalkSpeed;
	
	FVector Delta  = Velocity * deltaTime ;


	
	
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, NewRotation, true, Hit);
	
	if (Hit.bBlockingHit)
	{
		LastHitTest = Hit;
		DrawDebugCapsule(GetWorld(), UpdatedComponent->GetComponentLocation(), GetCapsuleHalfHeight(), GetCapsuleRadius(), UpdatedComponent->GetComponentQuat(),FColor::Red,false,0.0,1,1.0f);
	}
	else
	{
		Hit = GetFutureCollision(deltaTime);
		if (Hit.bBlockingHit)
		{
			//DrawDebugCapsule(GetWorld(), UpdatedComponent->GetComponentLocation(), GetCapsuleHalfHeight(), GetCapsuleRadius(), UpdatedComponent->GetComponentQuat(), FColor::Red, false,4, 1, 1.0f);
			
			//Adjust location on wall
			FHitResult OldHit = Hit;
			FVector Start = UpdatedComponent->GetComponentLocation();
			FVector End = Start + (UpdatedComponent->GetUpVector() * -1) * (GetCapsuleHalfHeight()*2);

			GetWorld()->SweepSingleByChannel(Hit, Start, End, UpdatedComponent->GetComponentQuat(), ECC_Visibility, FCollisionShape::MakeCapsule(10, GetCapsuleHalfHeight()));
			DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, -1.0f, 1, 1.0f);
			if (Hit.bBlockingHit)
			{
				FVector NewLocation = Hit.ImpactPoint + (UpdatedComponent->GetUpVector() * GetCapsuleHalfHeight());
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 15.0, 6, FColor::Red, false, -1.0f, 0, 1.0f);
				UpdatedComponent->SetWorldLocation(NewLocation);
			}
		}
		else
		{
			// On Cherche un mur


		}


		// We have to check if player is still near a wall
		// man this things challenge my sanity
	}
	

	


	
	
}

bool UCustomMovement::CanWalkOnWall()
{
	return true;
}

FHitResult UCustomMovement::GetFutureCollision(float DeltaSeconds)
{
	FHitResult Hit;
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + Velocity * DeltaSeconds;
	GetWorld()->SweepSingleByChannel(Hit, Start, End, UpdatedComponent->GetComponentQuat(), ECC_Visibility, FCollisionShape::MakeCapsule(GetCapsuleRadius(), GetCapsuleHalfHeight()));
	if (Hit.bBlockingHit)
	{

		return Hit;
	}
	return Hit;


}

FHitResult UCustomMovement::FindBaseWall()
{
	return FHitResult();
}

#pragma region Wallrun

void UCustomMovement::PhysWallRun(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	bJustTeleported = false;
	float remainingTime = deltaTime;
	const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
	
	FHitResult Hit(1.f);

	if (IsHittingWall(Hit) && IsWallRunnable(Hit))
	{

		const FVector OldVelocity = Velocity;	
		FVector WallRunDirection = GetWallRunDirection(Hit);
		float friction = 0.5f;

		FVector NewVelocity = ( WallRunDirection * Wall_RunMaxSpeed ) * timeTick;

		SafeMoveUpdatedComponent(NewVelocity * friction, CharacterOwner->GetActorRotation(), true, Hit);
		
			
	}
	else
	{
		SetMovementMode(MOVE_Walking);
		return;
	}

}

FVector UCustomMovement::CalcWallrunVelocity(float timeTick , FVector WallRunDirection)
{
	const FVector MoveVelocity = Velocity;
	const FVector Delta = timeTick * MoveVelocity;
		
	FVector NewVelocity = (WallRunDirection * Wall_RunMaxSpeed) * timeTick;

	return NewVelocity;
}

bool UCustomMovement::WantToWallRun()
{

	if (CanWallRun() && true)
	{
		return true;
	}
	return false;
}

bool UCustomMovement::CanWallRun()
{
	FHitResult Hit;
	if (IsHittingWall(Hit))
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("WallNormal : %s"), *Hit.Normal.ToString()));
		if (IsWallRunnable(Hit))
		{
			return true;
		}
	}
	return false;
}

bool UCustomMovement::IsWallRunnable(FHitResult Hit)
{
	return Hit.Normal.Z < WallRun_MaxAngle && Hit.Normal.Z > -WallRun_MinAngle;
}

bool UCustomMovement::IsHittingWall(FHitResult& Hit)
{
	if (CharacterOwner)
	{
		FVector StartLocation = CharacterOwner->GetActorLocation();
		//On trace un capsule plus grande que la capsule component
		UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(),StartLocation,StartLocation,
			GetCapsuleRadius()*WallRun_Scale.X,GetCapsuleHalfHeight()*WallRun_Scale.Y,
			ETraceTypeQuery::TraceTypeQuery1,false,
			TArray<AActor*>(),
			EDrawDebugTrace::ForDuration,
			Hit,true,
			FColor::Red,FColor::Green,GetComponentTickInterval());
		if (Hit.bBlockingHit)
		{
			return true;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("CharacterOwner is null")));
	}
	return false;
}

FVector UCustomMovement::GetWallRunDirection(FHitResult Hit)
{
	FVector Direction = Hit.Normal.GetSafeNormal();
	float Dot = FVector::DotProduct(Direction,CharacterOwner->GetActorForwardVector());
	FVector VelDir = (CharacterOwner->GetActorForwardVector() - ( Direction * Dot));


	UCameraComponent* CameraComponent = CharacterOwner->FindComponentByClass<UCameraComponent>();
	float CameraDot = FVector::DotProduct(Direction,CameraComponent->GetForwardVector());
	FVector VelDirCamera = (CameraComponent->GetForwardVector() - ( Direction * Dot));

	return VelDirCamera;
}

#pragma endregion

#pragma region CollisionTest

float UCustomMovement::GetCapsuleRadius()
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float UCustomMovement::GetCapsuleHalfHeight()
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}
#pragma endregion