// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompletedMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if(InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
		// Changes viewtarget to a spectating actor viewpoint once mission completed (set spectating actor class in Blueprints)
		if(SpectatingViewpointClass)
		{
			APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
			if(PC)
			{
				TArray<AActor*> SpectatingActors;
				UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, SpectatingActors);
				if(SpectatingActors.Num() > 0)
				{
					AActor* SpectatingActor = SpectatingActors[0];
					PC->SetViewTargetWithBlend(SpectatingActor, 0.7f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass not set in FPSGameMode"));
		}
	}

	OnMissionCompleted(InstigatorPawn, bMissionSuccess);

}