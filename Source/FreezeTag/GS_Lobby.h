// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GS_Lobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyUpdated);

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


public:
    UPROPERTY(BlueprintAssignable)
    FOnLobbyUpdated OnLobbyUpdated;

    void CheckAllReady();
    void NotifyLobbyChanged();
	
};
