// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stamina.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE          (FOnStaminaLossStarted								);
DECLARE_DYNAMIC_MULTICAST_DELEGATE          (FOnStaminaLossStoped								);
DECLARE_DYNAMIC_MULTICAST_DELEGATE          (FOnStaminaReachedZero								);
DECLARE_DYNAMIC_MULTICAST_DELEGATE          (FOnStaminaReachedMax								);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam	(FOnStaminaUpdated			, float, Stamina		);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam	(FOnStaminaLerpedUpdated	, float, StaminaLerped	);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POLADROID_API UStamina : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina" , BlueprintPure)
	float GetCurrentStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina" , BlueprintPure)
	float GetLerpedStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StartLoosingStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StartRegenStamina();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void PauseStamina();
	
private:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Stamina")
	float CurrentStamina;
	
	UPROPERTY(BlueprintReadWrite , EditDefaultsOnly , meta = (AllowPrivateAccess = "true"), Category = "Stamina | Lerp Settings")
	bool bEnableLerpedValue;
	
	UPROPERTY(BlueprintReadWrite , EditDefaultsOnly , meta = (AllowPrivateAccess = "true"), Category = "Stamina | Lerp Settings")
	float LerpSpeed;

	UPROPERTY(BlueprintReadWrite , EditDefaultsOnly , meta = (AllowPrivateAccess = "true" ,
		ToolTip = "	Stamina is updated 30 time per frames so StaminaRegenRate is added every 1/30"	), Category = "Stamina | Settings")
	float StaminaRegenRate;

	UPROPERTY(BlueprintReadWrite , EditDefaultsOnly , meta = (AllowPrivateAccess = "true" ,
		ToolTip = "	Stamina is updated 30 time per frames so StaminaRegenRate is added every 1/30"	), Category = "Stamina | Settings")
	float StaminaDecayRate;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Stamina")
	bool bIsLoosingStamina = false;
	
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true" , tooltip = " Called When Stamina Decreased"), Category = "Stamina | Events")
	FOnStaminaLossStarted OnStaminaLossStarted;
	
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true" , tooltip = " Called When Stamina Stopped Looping"), Category = "Stamina | Events")
	FOnStaminaLossStoped OnStaminaLossStoped;
	
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true" , tooltip = " Called When Stamina reaches MaxValue"), Category = "Stamina | Events")
	FOnStaminaReachedMax OnStaminaReachedMax;
	
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true" , tooltip = " Called When Stamina reaches zero"), Category = "Stamina | Events")
	FOnStaminaReachedZero OnStaminaReachedZero;
	
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true" , tooltip = " Called When Stamina is Updated"), Category = "Stamina | Events")
	FOnStaminaUpdated OnStaminaUpdated;
	
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = "true" , tooltip = " Called When Stamina Lerped Value is updated"), Category = "Stamina | Events")
	FOnStaminaLerpedUpdated OnStaminaLerpedUpdated;
	
	UPROPERTY()
	FTimerHandle TimerHandle_StaminaUpdate;

	UPROPERTY()
	float StaminaLerpedValue;
	
	UPROPERTY()
	bool bLoseStamina;

	UPROPERTY()
	float UpdateRate = 1 / 30.f;
	
	UFUNCTION()
	void UpdateStamina();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public://///
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the Owner take damage
	UFUNCTION()
	virtual void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
		
};
