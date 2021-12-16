// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensing->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensing->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();
	OnStateChanged(GuardState);

	if(bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();

		if(DistanceToGoal < 50)
		{
			MoveToNextPatrolPoint();
		}
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* PawnSeen)
{
	if(PawnSeen == nullptr) {return;}
	AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if(FPSGameMode)
	{
		FPSGameMode->CompletedMission(PawnSeen, false);
	}
	SetGuardState(EAIState::Alerted);

	AController* GuardController = GetController();
	if(GuardController)
	{
		GuardController->StopMovement();
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if(GuardState == EAIState::Alerted)
	{
		return;
	}

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NoiseLocation = FRotationMatrix::MakeFromX(Direction).Rotator();
	NoiseLocation.Pitch = 0.0f;
	NoiseLocation.Roll = 0.0f;

	SetActorRotation(NoiseLocation);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AFPSAIGuard::ResetRotation, GuardDistractTime);


	SetGuardState(EAIState::Distracted);

	AController* GuardController = GetController();
	if(GuardController)
	{
		GuardController->StopMovement();
	}

}


void AFPSAIGuard::ResetRotation()
{
	SetActorRotation(OriginalRotation);
	if(GuardState == EAIState::Alerted)
	{
		return;
	}
	SetGuardState(EAIState::Idle);

	if(bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if(GuardState == NewState) {return;}

	GuardState = NewState;
	OnStateChanged(GuardState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if(CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		
		CurrentPatrolPoint = SecondPatrolPoint;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

