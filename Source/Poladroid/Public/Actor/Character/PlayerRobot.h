// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/InterfaceLightLevel.h"
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

	UPROPERTY(EditAnywhere, Category = "LightLevel", meta = (AllowPrivateAccess = "true" ,ToolTip = "Get the current light level"))
	FVector2D LightLevelRange;	//min and max light level

	UFUNCTION(BlueprintCallable, Category = "LightLevel")
	virtual float GetLightLevel_Implementation() override {return CurrentLightLevel;}

	UFUNCTION() 
	float CalcLightLevel() const;

	UFUNCTION(BlueprintCallable, Category = "LightLevel" , meta = (AllowPrivate))
	float RemapLightLevel(float from, float fromMin, float fromMax, float toMin,  float toMax) const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact(const FInputActionValue& Value);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
