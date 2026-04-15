// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"
#include "PS_Lobby.h"
#include "GS_Lobby.h"
#include "GI_FreezeTag.h"
#include "GM_Lobby.h"
#include "MultiplayerSessionManager.h"

void APC_Lobby::RequestSetReady(bool bReady)
{
    ServerSetReady(bReady);
}

void APC_Lobby::ServerSetReady_Implementation(bool bReady)
{
    if (APS_Lobby* LPS = GetPlayerState<APS_Lobby>())
        LPS->SetReady(bReady);
}

void APC_Lobby::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        if (LobbyWidgetClass)
        {
            LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
            if (LobbyWidget)
            {
                LobbyWidget->AddToViewport();
                SetInputMode(FInputModeUIOnly());
                bShowMouseCursor = true;
            }
        }
    }

    if (UGI_FreezeTag* GI = GetGameInstance<UGI_FreezeTag>())
    {
        if (HasAuthority())
        {
            if (APlayerState* PS = GetPlayerState<APlayerState>())
            {
                PS->SetPlayerName(GI->LocalPlayerName);
                if (AGS_Lobby* LGS = GetWorld()->GetGameState<AGS_Lobby>())
                    LGS->NotifyLobbyChanged();
            }
        }
        else
        {
            ServerSetPlayerName(GI->LocalPlayerName);
        }
    }
}

void APC_Lobby::ServerSetPlayerName_Implementation(const FString& NewName)
{
    if (APlayerState* PS = GetPlayerState<APlayerState>())
    {
        PS->SetPlayerName(NewName);
        if (AGS_Lobby* LGS = GetWorld()->GetGameState<AGS_Lobby>())
            LGS->NotifyLobbyChanged();
    }
}

void APC_Lobby::RequestQuit()
{
    ServerQuit();
}

void APC_Lobby::ServerQuit_Implementation()
{
    if (IsLocalController())
    {
        if (AGM_Lobby* GM = GetWorld()->GetAuthGameMode<AGM_Lobby>())
            GM->EvictAllPlayers();
    }
    else
    {
        ClientQuit();
    }
}

void APC_Lobby::ClientQuit_Implementation()
{
    if (UMultiplayerSessionManager* SM = GetGameInstance()->GetSubsystem<UMultiplayerSessionManager>())
    {
        SM->OnDestroySessionComplete.AddDynamic(this, &APC_Lobby::OnSessionDestroyed);
        SM->DestroySession();
    }
}

void APC_Lobby::OnSessionDestroyed()
{
    if (UMultiplayerSessionManager* SM = GetGameInstance()->GetSubsystem<UMultiplayerSessionManager>())
        SM->OnDestroySessionComplete.RemoveDynamic(this, &APC_Lobby::OnSessionDestroyed);

    ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}