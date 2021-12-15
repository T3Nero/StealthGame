// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	UFUNCTION()
	void OverlapBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* LaunchEffect;

	// Amount of Velocity to add when Launching an Actor/Character
	UPROPERTY(EditInstanceOnly)
	float LaunchStrength;

	// Angle added to actor rotation to launch in a specific direction
	UPROPERTY(EditInstanceOnly)
	float LaunchAngle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


