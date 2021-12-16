// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Distracted,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
	void OnPawnSeen(APawn* PawnSeen);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	FTimerHandle TimerHandle_ResetRotation;

	UPROPERTY(EditAnywhere)
	float GuardDistractTime = 3.0f;

	UFUNCTION()
	void ResetRotation();

	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStateChanged(EAIState NewState);

	UPROPERTY(EditInstanceOnly)
	bool bPatrol;

	AActor* CurrentPatrolPoint;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bPatrol"))
	AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bPatrol"))
	AActor* SecondPatrolPoint;

	void MoveToNextPatrolPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
