// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/InterfaceLightLevel.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerRobot.generated.h"

class UInputComponent;

UCLASS()
class POLADROID_API APlayerRobot : public ACharacter , public IInterfaceLightLevel
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction*			JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction*			MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction*			InteractAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction*			LookAction;

public:
	// Sets default values for this character's properties
	APlayerRobot(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region LightLevel
private:
	//CAPTURE COMPONENT 2D
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightLevel", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* CaptureComponentUp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightLevel", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* CaptureComponentDown;
	
	UPROPERTY(EditAnywhere, Category = "LightLevel", meta = (AllowPrivateAccess = "true", ToolTip = "The render target to read the light level"))
	UTextureRenderTarget2D* RenderTargetMain;
	UPROPERTY(EditAnywhere, Category = "LightLevel", meta = (AllowPrivateAccess = "true", ToolTip = "Material we are going to use to calculate the light level"))
	UMaterialInterface* MaterialForCalcLightLevel;
	//Sphere to render the light level
	UPROPERTY(EditAnywhere, Category = "LightLevel")
	class UStaticMeshComponent* SphereForLighting;

	UPROPERTY(EditAnywhere , Category = "LightLevel" , meta = (AllowPrivateAccess = "true"))
	float CurrentLightLevel;

	UPROPERTY(EditAnywhere, Category = "LightLevel", meta = (AlalowPrivateAccess = "true" ,ToolTip = "Get the current light level"))
	FVector2D LightLevelRange;	//min and max light level

	UPROPERTY(EditAnywhere, Category = "LightLevel", meta = (AlalowPrivateAccess = "true" ,ToolTip = "Get the current light level"))
	TMap<ELightLevel,float> LightLevelMap;
	
	UFUNCTION(BlueprintCallable, Category = "LightLevel")
	virtual float GetLightLevelValue_Implementation() override { return UKismetMathLibrary::FClamp(RemapLightLevel(CurrentLightLevel, LightLevelRange.X, LightLevelRange.Y , 0.0f,1.0f ),0,1);}

	UFUNCTION(BlueprintCallable, Category = "LightLevel")
	virtual ELightLevel GetLightLevel_Implementation() override;
	
	UFUNCTION(BlueprintCallable, Category = "LightLevel" , meta = (AllowPrivate))
	float GetRawLightLevel() const {return CurrentLightLevel;}
	
	UFUNCTION() 
	float CalcLightLevel() const;

	UFUNCTION(BlueprintCallable, Category = "LightLevel" , meta = (AllowPrivate))
	float RemapLightLevel(float from, float fromMin, float fromMax, float toMin,  float toMax) const;

#pragma endregion

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	AActor* RotationPoint = nullptr;

	// Camera interpolation movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraRotationSpeed = 10;
	
protected:	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact(const FInputActionValue& Value);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Call when the player touch it
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public: //Sadly we need to make this public to be able to call it from Blueprints
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Respawn" , meta = (DisplayName = "OnRespawn" , AllowPrivateAccess = "true" , ToolTip = "Called when the player respawns"))
	void OnRespawn();
	virtual void OnRespawn_Implementation();
};
