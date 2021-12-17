// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

// GameState must be used instead of GameMode when running code on SERVER & all Clients as GameMode is SERVER only
// Multicast is used so we can run the code on all Clients & SERVER at once (Multiplayer)
void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
    for(FConstPlayerControllerIterator IT = GetWorld()->GetPlayerControllerIterator(); IT; IT++)
    {
        AFPSPlayerController* PC = Cast<AFPSPlayerController>(IT->Get());
        // Checks if LocalController so code is run only on the owning controller
        if(PC && PC->IsLocalController())
        {
            PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
            APawn* Pawn = PC->GetPawn();
            if(Pawn)
            {
                Pawn->DisableInput(PC);
            }
        }
    }
}
