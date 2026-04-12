// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"
#include "PS_Lobby.h"
#include "GI_FreezeTag.h"

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

    if (!IsLocalController()) return;

    if (UGI_FreezeTag* GI = GetGameInstance<UGI_FreezeTag>())
    {
        ServerSetPlayerName(GI->LocalPlayerName);
    }
}

void APC_Lobby::ServerSetPlayerName_Implementation(const FString& NewName)
{
    if (APS_Lobby* PS = GetPlayerState<APS_Lobby>())
        PS->SetPlayerName(NewName);
}