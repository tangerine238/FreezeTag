// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Lobby.h"
#include "GS_Lobby.h"
#include "Net/UnrealNetwork.h"



void APS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APS_Lobby, bIsReady);
}

void APS_Lobby::SetReady(bool bReady)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        bIsReady = bReady;
        if (AGS_Lobby* LGS = GetWorld()->GetGameState<AGS_Lobby>())
        {
            LGS->CheckAllReady();
        }
    }
}

void APS_Lobby::OnRep_IsReady()
{
    // UI will refresh via OnLobbyUpdated delegate
}