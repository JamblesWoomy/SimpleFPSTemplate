#include "FPSGameState.h"

#include "FPSPlayerController.h"

void AFPSGameState::MultiCastOnMissionComplete_Implementation(APawn* instigatorPawn) {
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		AFPSPlayerController* pController = Cast<AFPSPlayerController>(it->Get());
		if (pController && pController->IsLocalController()) {
			pController->OnMissionCompleted(instigatorPawn);
			APawn* mPawn = pController->GetPawn();
			if (mPawn) {
				mPawn->DisableInput(pController);
			}
		}
	}
}