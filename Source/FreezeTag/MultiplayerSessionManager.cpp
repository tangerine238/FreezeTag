// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerSessionManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionManager::UMultiplayerSessionManager() : CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted)),
                                                           FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted)),
                                                           JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted)),
                                                           DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted))
{
    if (IOnlineSubsystem *OSS = IOnlineSubsystem::Get())
    {
        SessionInterface = OSS->GetSessionInterface();
    }
}

void UMultiplayerSessionManager::CreateSession(int32 NumPublicConnections, FString MatchType)
{
    if (!SessionInterface.IsValid())
        return;

    if (SessionInterface->GetNamedSession(NAME_GameSession))
    {
        SessionInterface->DestroySession(NAME_GameSession);
    }

    CreateSessionHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
    LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
    LastSessionSettings->NumPublicConnections = NumPublicConnections;
    LastSessionSettings->bAllowJoinInProgress = true;
    LastSessionSettings->bAllowJoinViaPresence = true;
    LastSessionSettings->bShouldAdvertise = true;
    LastSessionSettings->bUsesPresence = true;
    LastSessionSettings->bUseLobbiesIfAvailable = true;
    LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    LastSessionSettings->Set(FName("ServerName"), FString("Stefan's Game"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
        OnCreateSessionComplete.Broadcast(false);
    }
}

void UMultiplayerSessionManager::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface)
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
    OnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionManager::FindSessions(int32 MaxSearchResults)
{
    if (!SessionInterface.IsValid())
        return;

    FindSessionsHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = MaxSearchResults;
    SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
    SessionSearch->QuerySettings.Set(FName(TEXT("sessionpresence")), true, EOnlineComparisonOp::Equals);

    const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef()))
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsHandle);
        OnFindSessionsCompleted(false);
    }
}

void UMultiplayerSessionManager::JoinSession(const FOnlineSessionSearchResult &SessionResult)
{
    if (!SessionInterface.IsValid())
        return;

    JoinSessionHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

    const ULocalPlayer *LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult);
}

void UMultiplayerSessionManager::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface)
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionHandle);

    bool bSuccess = Result == EOnJoinSessionCompleteResult::Success;
    if (bSuccess)
    {
        FString Address;
        SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
        APlayerController *PC = GetWorld()->GetFirstPlayerController();
        PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
    }

    OnJoinSessionComplete.Broadcast(bSuccess);
}

void UMultiplayerSessionManager::DestroySession()
{
    if (!SessionInterface.IsValid())
        return;
    DestroySessionHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
    SessionInterface->DestroySession(NAME_GameSession);
}

void UMultiplayerSessionManager::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface)
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionHandle);
    OnDestroySessionComplete.Broadcast();
}

void UMultiplayerSessionManager::OnFindSessionsCompleted(bool bWasSuccessful)
{
    if (SessionInterface)
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsHandle);

    if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
    {
        LastSearchResults = SessionSearch->SearchResults;
    }

    OnFindSessionsComplete.Broadcast(bWasSuccessful);
}