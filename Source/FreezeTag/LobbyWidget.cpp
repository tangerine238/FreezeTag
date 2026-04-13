// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "GS_Lobby.h"
#include "PS_Lobby.h"
#include "PC_Lobby.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ReadyButton)
        ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnReadyClicked);

    if (AGS_Lobby* LGS = GetWorld()->GetGameState<AGS_Lobby>())
        LGS->OnLobbyUpdated.AddDynamic(this, &ULobbyWidget::RefreshPlayerList);


    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULobbyWidget::RefreshPlayerList, 0.5f, false);
}

void ULobbyWidget::OnReadyClicked()
{
    bIsReady = !bIsReady;
    
    if (ReadyButtonText){
        
        ReadyButtonText->SetText(FText::FromString(bIsReady ? TEXT("Ready") : TEXT("Not Ready")));
    }

    if (APC_Lobby* PC = Cast<APC_Lobby>(GetOwningPlayer()))
        PC->RequestSetReady(bIsReady);
}

void ULobbyWidget::RefreshPlayerList()
{
    if (!PlayerList) return;
    PlayerList->ClearChildren();

    AGS_Lobby* LGS = GetWorld()->GetGameState<AGS_Lobby>();
    if (!LGS) return;

    for (APlayerState* PS : LGS->PlayerArray)
    {
        APS_Lobby* LPS = Cast<APS_Lobby>(PS);
        if (!LPS) continue;

        FString PlayerName = LPS->GetPlayerName();
        FString Status = LPS->bIsReady ? TEXT("Ready") : TEXT("Not Ready");
        FString Line = FString::Printf(TEXT("%s - %s"), *PlayerName, *Status);

        UTextBlock* Entry = NewObject<UTextBlock>(this);
        Entry->SetText(FText::FromString(Line));
        PlayerList->AddChild(Entry);
    }
}