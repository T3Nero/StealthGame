// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	AttractSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Attract Sphere Comp"));
	AttractSphereComp->SetSphereRadius(3000);
	AttractSphereComp->SetupAttachment(MeshComp);


	DestroySphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Destroy Sphere Comp"));
	DestroySphereComp->SetSphereRadius(100);
	DestroySphereComp->SetupAttachment(MeshComp);
	
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	//Bind To Function
	DestroySphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapDestroySphere);
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	AttractSphereComp->GetOverlappingComponents(OverlappingComps);

	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if(PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = AttractSphereComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
	
	


}

void ABlackHole::OverlapDestroySphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor)
	{
		OtherActor->Destroy();
	}
}

