// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "FPSCharacter.h"


// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects();
	
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickUpFX, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor);
	if(Player)
	{
		Player->bIsCarryingObjective = true;
		Destroy();
	}
}

