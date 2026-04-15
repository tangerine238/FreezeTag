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

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnQuitClicked);

    if (AGS_Lobby* LGS = GetWorld()->GetGameState<AGS_Lobby>())
    {
        LGS->OnLobbyUpdated.AddDynamic(this, &ULobbyWidget::RefreshPlayerList);
        LGS->OnGameStarting.AddDynamic(this, &ULobbyWidget::ShowLoadingScreen);
    }


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

    TArray<APlayerState*> SortedPlayers = LGS->PlayerArray;
    SortedPlayers.Sort([](const APlayerState& A, const APlayerState& B)
        {
            return A.GetPlayerId() < B.GetPlayerId();
        });

    for (APlayerState* PS : SortedPlayers)
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


void ULobbyWidget::ShowLoadingScreen()
{
    if (LoadingScreen)
        LoadingScreen->SetVisibility(ESlateVisibility::Visible);
    
    if (LoadingText)
        LoadingText->SetText(FText::FromString(TEXT("Starting Game...")));
}


void ULobbyWidget::OnQuitClicked()
{
    if (APC_Lobby* PC = Cast<APC_Lobby>(GetOwningPlayer()))
        PC->RequestQuit();
}