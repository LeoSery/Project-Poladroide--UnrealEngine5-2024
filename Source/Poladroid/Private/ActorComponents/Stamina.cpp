// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/Stamina.h"

#include "DamgeTypes/StaminaDamage.h"

// Sets default values for this component's properties
UStamina::UStamina()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UStamina::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStamina::OnOwnerTakeDamage);
	// ...
	
}

float UStamina::GetCurrentStamina()
{
	return CurrentStamina;
}

float UStamina::GetLerpedStamina()
{
	return StaminaLerpedValue;
}

void UStamina::StartLoosingStamina()
{
	bLoseStamina = true;

	if (! TimerHandle_StaminaUpdate.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaUpdate, this, &UStamina::UpdateStamina, UpdateRate, true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Timer Started"));
	}
	if (GetWorld()->GetTimerManager().IsTimerPaused(TimerHandle_StaminaUpdate))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle_StaminaUpdate);
	}
	this->SetComponentTickEnabled(true);
	OnStaminaLossStarted.Broadcast();
}

void UStamina::StartRegenStamina()
{
	bLoseStamina = false;

	if (!TimerHandle_StaminaUpdate.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_StaminaUpdate, this, &UStamina::UpdateStamina, UpdateRate, true);
	}
	if (GetWorld()->GetTimerManager().IsTimerPaused(TimerHandle_StaminaUpdate))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle_StaminaUpdate);
	}
	this->SetComponentTickEnabled(true);
	OnStaminaLossStoped.Broadcast();
}


void UStamina::UpdateStamina()
{
	float AddedStamina = bLoseStamina ? -StaminaDecayRate : StaminaRegenRate;

	float FutureStamina = CurrentStamina + AddedStamina;

	if (FutureStamina < 0)
	{
		CurrentStamina = 0;
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle_StaminaUpdate);
		OnStaminaReachedZero.Broadcast();
	}
	else if (FutureStamina > 1)
	{
		CurrentStamina = 1;
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle_StaminaUpdate);
		OnStaminaReachedMax.Broadcast();
	}
	else
	{
		CurrentStamina = FutureStamina;
	}
	OnStaminaUpdated.Broadcast(CurrentStamina);
	
}

// Called every frame
void UStamina::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Hi i use this tick to update the lerped value of the stamina
	if (bEnableLerpedValue)
	{
		StaminaLerpedValue = FMath::FInterpTo(StaminaLerpedValue, CurrentStamina, DeltaTime, LerpSpeed);
		OnStaminaLerpedUpdated.Broadcast(StaminaLerpedValue);

		if (StaminaLerpedValue == CurrentStamina && GetWorld()->GetTimerManager().IsTimerPaused(TimerHandle_StaminaUpdate))
		{
			this->SetComponentTickEnabled(false);
		}
	}
}

void UStamina::OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Owner Take Damage"));
	//Cast DamageType to DamageTypeToRemoveStamina;
	if (const UStaminaDamage* DamageTypeToRemoveStamina = Cast<UStaminaDamage>(DamageType))
	{
		SetComponentTickEnabled(false);
		CurrentStamina = 0;
		StaminaLerpedValue = CurrentStamina;
		OnStaminaReachedZero.Broadcast();
		OnStaminaLerpedUpdated.Broadcast(StaminaLerpedValue);
		OnStaminaUpdated.Broadcast(CurrentStamina);
		StartRegenStamina();
	}
}

