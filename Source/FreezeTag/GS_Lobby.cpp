// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Lobby.h"
#include "PS_Lobby.h"
#include "GM_Lobby.h"

void AGS_Lobby::CheckAllReady()
{
    if (PlayerArray.Num() == 0) return;

    bool bAllReady = true;
    for (APlayerState* PS : PlayerArray)
    {
        APS_Lobby* LPS = Cast<APS_Lobby>(PS);
        if (!LPS || !LPS->bIsReady)
        {
            bAllReady = false;
            break;
        }
    }

    if (bAllReady)
    {
		// create loading screen here
        if (AGM_Lobby* LGM = GetWorld()->GetAuthGameMode<AGM_Lobby>())
        {
            LGM->StartGame();
        }
    }

    NotifyLobbyChanged();
}

void AGS_Lobby::NotifyLobbyChanged()
{
    MulticastNotifyLobbyChanged();
}

void AGS_Lobby::MulticastNotifyLobbyChanged_Implementation()
{
    OnLobbyUpdated.Broadcast();
}

