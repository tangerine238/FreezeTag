// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GS_Lobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStarting);

/**
 * 
 */
UCLASS()
class FREEZETAG_API AGS_Lobby : public AGameState
{
	GENERATED_BODY()


protected:
    UFUNCTION(NetMulticast, Reliable)
    void MulticastNotifyLobbyChanged();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastNotifyGameStarting();


public:
    UPROPERTY(BlueprintAssignable)
    FOnLobbyUpdated OnLobbyUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnGameStarting OnGameStarting;

    void CheckAllReady();
    void NotifyLobbyChanged();
	
};
