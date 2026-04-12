// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Lobby.h"
#include "GS_Lobby.h"
#include "PS_Lobby.h"
#include "PC_Lobby.h"

AGM_Lobby::AGM_Lobby()
{
    PlayerStateClass = APS_Lobby::StaticClass();
    GameStateClass = AGS_Lobby::StaticClass();
    PlayerControllerClass = APC_Lobby::StaticClass();
}

void AGM_Lobby::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (AGS_Lobby* LGS = GetGameState<AGS_Lobby>())
        LGS->NotifyLobbyChanged();
}

void AGM_Lobby::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    // Reset all ready states when someone leaves
    if (AGS_Lobby* LGS = GetGameState<AGS_Lobby>())
    {
        for (APlayerState* PS : LGS->PlayerArray)
        {
            if (APS_Lobby* LPS = Cast<APS_Lobby>(PS))
                LPS->SetReady(false);
        }
        LGS->NotifyLobbyChanged();
    }
}

void AGM_Lobby::StartGame()
{
    if (GetNumPlayers() < MinPlayersToStart) return;
    GetWorld()->ServerTravel(GameMapPath + TEXT("?listen"), true);
}