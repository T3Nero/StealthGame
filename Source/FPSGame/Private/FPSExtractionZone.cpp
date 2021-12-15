// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Component"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.f);
	DecalComp->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
	
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	AFPSCharacter* PlayerPawn = Cast<AFPSCharacter>(OtherActor);
	if(PlayerPawn == nullptr) {return;}

	if(PlayerPawn->bIsCarryingObjective)
	{
		AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if(FPSGameMode)
		{
			FPSGameMode->CompletedMission(PlayerPawn);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}

