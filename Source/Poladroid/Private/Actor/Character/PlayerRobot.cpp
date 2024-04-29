// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/PlayerRobot.h"
#include "ActorComponents/CustomMovement.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values
APlayerRobot::APlayerRobot(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovement>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereForLighting = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereForLighting"));
	SphereForLighting->SetupAttachment(RootComponent);

	//Set Static mesh to Sphere
	const ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	SphereForLighting->SetStaticMesh(SphereMesh.Object);
	
	SphereForLighting->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	SphereForLighting->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereForLighting->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CaptureComponentUp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponentUp"));
	CaptureComponentUp->SetupAttachment(SphereForLighting);
	CaptureComponentUp->SetRelativeRotation(FRotator(-90, 0, 0));
	CaptureComponentUp->SetRelativeLocation(FVector(0.0f,0.0f,60.0f));
	CaptureComponentUp->ProjectionType = ECameraProjectionMode::Orthographic;
	CaptureComponentUp->OrthoWidth = 50.0f;

	#pragma region Set Show Flags CaptureComponentDown

	CaptureComponentUp->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_LegacySceneCapture;
	CaptureComponentUp->bCaptureEveryFrame = true;
	CaptureComponentUp->bCaptureOnMovement = false;
	CaptureComponentUp->MaxViewDistanceOverride = 25.0f;
	CaptureComponentUp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	
	CaptureComponentUp->ShowFlags.Atmosphere				= false;
	CaptureComponentUp->ShowFlags.BSP						= false;
	CaptureComponentUp->ShowFlags.Cloud					= false;
	CaptureComponentUp->ShowFlags.Decals					= false;
	CaptureComponentUp->ShowFlags.Fog						= false;
	CaptureComponentUp->ShowFlags.Landscape				= false;
	CaptureComponentUp->ShowFlags.Particles				= false;
	CaptureComponentUp->ShowFlags.SkeletalMeshes			= false;
	CaptureComponentUp->ShowFlags.StaticMeshes			= true;
	CaptureComponentUp->ShowFlags.Translucency			= false;

	CaptureComponentUp->ShowFlags.DeferredLighting		= true;
	CaptureComponentUp->ShowFlags.InstancedFoliage		= false;
	CaptureComponentUp->ShowFlags.InstancedGrass			= false;
	CaptureComponentUp->ShowFlags.InstancedStaticMeshes	= false;
	CaptureComponentUp->ShowFlags.Paper2DSprites			= false;
	CaptureComponentUp->ShowFlags.TextRender				= false;
	CaptureComponentUp->ShowFlags.TemporalAA				= false;

	CaptureComponentUp->ShowFlags.Bloom					= false;
	CaptureComponentUp->ShowFlags.EyeAdaptation			= false;
	CaptureComponentUp->ShowFlags.LocalExposure			= false;
	CaptureComponentUp->ShowFlags.MotionBlur				= false;
	CaptureComponentUp->ShowFlags.ToneCurve				= false;

	CaptureComponentUp->ShowFlags.SkyLighting				= false;
	CaptureComponentUp->ShowFlags.AmbientOcclusion		= true;
	CaptureComponentUp->ShowFlags.DynamicShadows			= true;

	CaptureComponentUp->ShowFlags.AmbientCubemap			= true;
	CaptureComponentUp->ShowFlags.DistanceFieldAO			= true;
	CaptureComponentUp->ShowFlags.LightFunctions			= true;
	CaptureComponentUp->ShowFlags.LightShafts				= true;
	CaptureComponentUp->ShowFlags.ReflectionEnvironment	= true;

	CaptureComponentUp->ShowFlags.ScreenSpaceReflections	= false;
	CaptureComponentUp->ShowFlags.TexturedLightProfiles	= false;
	CaptureComponentUp->ShowFlags.VolumetricFog			= false;

	CaptureComponentUp->ShowFlags.NaniteMeshes			= true;
	CaptureComponentUp->ShowFlags.Game					= false;
	CaptureComponentUp->ShowFlags.Lighting				= true;
	CaptureComponentUp->ShowFlags.PathTracing				= false;
	CaptureComponentUp->ShowFlags.PostProcessing			= true;

#pragma endregion

	CaptureComponentDown = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponentDown"));
	CaptureComponentDown->SetupAttachment(SphereForLighting);
	CaptureComponentDown->SetRelativeRotation(FRotator(90, 0, 0));
	CaptureComponentDown->SetRelativeLocation(FVector(0.0f,0.0f,-60.0f));
	CaptureComponentDown->ProjectionType = ECameraProjectionMode::Orthographic;
	CaptureComponentDown->OrthoWidth = 50.0f;

	#pragma region Set Show Flags CaptureComponentUp

	CaptureComponentDown->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_LegacySceneCapture;
	CaptureComponentDown->bCaptureEveryFrame = true;
	CaptureComponentDown->bCaptureOnMovement = false;
	CaptureComponentDown->MaxViewDistanceOverride = 25.0f;
	CaptureComponentDown->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	
	CaptureComponentDown->ShowFlags.Atmosphere				= false;
	CaptureComponentDown->ShowFlags.BSP						= false;
	CaptureComponentDown->ShowFlags.Cloud					= false;
	CaptureComponentDown->ShowFlags.Decals					= false;
	CaptureComponentDown->ShowFlags.Fog						= false;
	CaptureComponentDown->ShowFlags.Landscape				= false;
	CaptureComponentDown->ShowFlags.Particles				= false;
	CaptureComponentDown->ShowFlags.SkeletalMeshes			= false;
	CaptureComponentDown->ShowFlags.StaticMeshes			= true;
	CaptureComponentDown->ShowFlags.Translucency			= false;

	CaptureComponentDown->ShowFlags.DeferredLighting		= true;
	CaptureComponentDown->ShowFlags.InstancedFoliage		= false;
	CaptureComponentDown->ShowFlags.InstancedGrass			= false;
	CaptureComponentDown->ShowFlags.InstancedStaticMeshes	= false;
	CaptureComponentDown->ShowFlags.Paper2DSprites			= false;
	CaptureComponentDown->ShowFlags.TextRender				= false;
	CaptureComponentDown->ShowFlags.TemporalAA				= false;

	CaptureComponentDown->ShowFlags.Bloom					= false;
	CaptureComponentDown->ShowFlags.EyeAdaptation			= false;
	CaptureComponentDown->ShowFlags.LocalExposure			= false;
	CaptureComponentDown->ShowFlags.MotionBlur				= false;
	CaptureComponentDown->ShowFlags.ToneCurve				= false;

	CaptureComponentDown->ShowFlags.SkyLighting				= false;
	CaptureComponentDown->ShowFlags.AmbientOcclusion		= true;
	CaptureComponentDown->ShowFlags.DynamicShadows			= true;

	CaptureComponentDown->ShowFlags.AmbientCubemap			= true;
	CaptureComponentDown->ShowFlags.DistanceFieldAO			= true;
	CaptureComponentDown->ShowFlags.LightFunctions			= true;
	CaptureComponentDown->ShowFlags.LightShafts				= true;
	CaptureComponentDown->ShowFlags.ReflectionEnvironment	= true;

	CaptureComponentDown->ShowFlags.ScreenSpaceReflections	= false;
	CaptureComponentDown->ShowFlags.TexturedLightProfiles	= false;
	CaptureComponentDown->ShowFlags.VolumetricFog			= false;

	CaptureComponentDown->ShowFlags.NaniteMeshes			= true;
	CaptureComponentDown->ShowFlags.Game					= false;
	CaptureComponentDown->ShowFlags.Lighting				= true;
	CaptureComponentDown->ShowFlags.PathTracing				= false;
	CaptureComponentDown->ShowFlags.PostProcessing			= true;

#pragma endregion
	
	LightLevelMap.Add(ELightLevel::LightLevel_Low, 0.0f);//if superiror to this value, the light level is Medium else it's Low
	LightLevelMap.Add(ELightLevel::LightLevel_Medium , 0.0f); // if superior to this value, the light level is High else it's Medium
	
}

ELightLevel APlayerRobot::GetLightLevel_Implementation()
{
	if (GetLightLevelValue_Implementation() < LightLevelMap[ELightLevel::LightLevel_Low])
	{
		return ELightLevel::LightLevel_Low;
	}
	else if (GetLightLevelValue_Implementation() < LightLevelMap[ELightLevel::LightLevel_Medium])
	{
		return ELightLevel::LightLevel_Medium;
	}
	else
	{
		return ELightLevel::LightLevel_High;
	}
}

float APlayerRobot::CalcLightLevel() const
{
	if (RenderTargetMain != nullptr)
	{
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), RenderTargetMain , MaterialForCalcLightLevel);
		return UKismetRenderingLibrary::ReadRenderTargetRawPixel(GetWorld(),RenderTargetMain,0,0,false).R;
	}
	//Calc Light Level
	return -1.0f;
}

float APlayerRobot::RemapLightLevel(float value, float fromMin, float fromMax, float toMin,  float toMax) const
{
	float fromAbs  =  value - fromMin;
	float fromMaxAbs = fromMax - fromMin;      
       
	float normal = fromAbs / fromMaxAbs;
 
	float toMaxAbs = toMax - toMin;
	float toAbs = toMaxAbs * normal;
 
	return toAbs + toMin;
}

// Called when the game starts or when spawned
void APlayerRobot::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerRobot::OnOverlapBegin);
	}
}

// Called every frame
void APlayerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentLightLevel = CalcLightLevel();
}

void APlayerRobot::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APlayerRobot::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerRobot::Interact(const FInputActionValue& Value)
{
	
}

// Called to bind functionality to input
void APlayerRobot::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerRobot::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerRobot::Look);
		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerRobot::Interact);
	}
	
}

void APlayerRobot::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("CameraPoint"))
	{
		RotationPoint = OtherActor;
	}
}

void APlayerRobot::OnRespawn_Implementation()
{
}
