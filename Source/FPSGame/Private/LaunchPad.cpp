// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComponent = MeshComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxComp->SetupAttachment(MeshComp);

	LaunchStrength = 1500.0f;
	LaunchAngle = 35.0f;
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OverlapBoxComponent);
	
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaunchPad::OverlapBoxComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchAngle;

	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;
	
	AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(OtherActor);
	if(OtherActor == FPSCharacter)
	{
		FPSCharacter->LaunchCharacter(LaunchVelocity, true, true);
	}
	else if(OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchEffect, GetActorLocation());
}

