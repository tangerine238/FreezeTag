// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Lobby.h"
#include "GS_Lobby.h"
#include "PS_Lobby.h"
#include "PC_Lobby.h"
#include "MultiplayerSessionManager.h"

AGM_Lobby::AGM_Lobby()
{
    PlayerStateClass = APS_Lobby::StaticClass();
    GameStateClass = AGS_Lobby::StaticClass();
    PlayerControllerClass = APC_Lobby::StaticClass();
}

void AGM_Lobby::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (AGS_Lobby* LGS = GetGameState<AGS_Lobby>())
                LGS->NotifyLobbyChanged();
        }, 0.5f, false);
}

void AGM_Lobby::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (AGS_Lobby* LGS = GetGameState<AGS_Lobby>())
            {
                // Reset all ready states
                for (APlayerState* PS : LGS->PlayerArray)
                {
                    if (APS_Lobby* LPS = Cast<APS_Lobby>(PS))
                        LPS->SetReady(false);
                }
                LGS->NotifyLobbyChanged();
            }
        }, 0.2f, false);
}

void AGM_Lobby::StartGame()
{
    if (GetNumPlayers() < MinPlayersToStart) return;
    GetWorld()->ServerTravel(GameMapPath + TEXT("?listen"), true);
}


void AGM_Lobby::EvictAllPlayers()
{
    if (bIsEvicting) return;
    bIsEvicting = true;

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && !PC->IsLocalController())
        {
            if (APC_Lobby* LPC = Cast<APC_Lobby>(PC))
                LPC->ClientQuit();
        }
    }

    if (UMultiplayerSessionManager* SM = GetGameInstance()->GetSubsystem<UMultiplayerSessionManager>())
    {
        SM->OnDestroySessionComplete.AddDynamic(this, &AGM_Lobby::OnSessionDestroyed);
        SM->DestroySession();
    }
}

void AGM_Lobby::OnSessionDestroyed()
{
    if (UMultiplayerSessionManager* SM = GetGameInstance()->GetSubsystem<UMultiplayerSessionManager>())
        SM->OnDestroySessionComplete.RemoveDynamic(this, &AGM_Lobby::OnSessionDestroyed);

    APlayerController* HostPC = GetWorld()->GetFirstPlayerController();
    if (HostPC)
        HostPC->ClientTravel(MainMenuMap, ETravelType::TRAVEL_Absolute);
}