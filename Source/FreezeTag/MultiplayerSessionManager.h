// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFTOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFTOnJoinSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFTOnDestroySessionComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFTOnFindSessionsComplete, bool, bWasSuccessful);


/**
 *
 */
UCLASS()
class FREEZETAG_API UMultiplayerSessionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionManager();

	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult &SessionResult);
	void DestroySession();

	FFTOnCreateSessionComplete OnCreateSessionComplete;
	FFTOnJoinSessionComplete OnJoinSessionComplete;
	FFTOnDestroySessionComplete OnDestroySessionComplete;
	FFTOnFindSessionsComplete OnFindSessionsComplete;

	TArray<FOnlineSessionSearchResult> LastSearchResults;

	

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsCompleted(bool bWasSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	

	FDelegateHandle CreateSessionHandle;
	FDelegateHandle FindSessionsHandle;
	FDelegateHandle JoinSessionHandle;
	FDelegateHandle DestroySessionHandle;
};
