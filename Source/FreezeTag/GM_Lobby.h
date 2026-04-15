// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class FREEZETAG_API AGM_Lobby : public AGameMode
{
	GENERATED_BODY()

public:
    AGM_Lobby();

    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    void StartGame();

    UPROPERTY(EditDefaultsOnly)
    int32 MinPlayersToStart = 2;

    UPROPERTY(EditDefaultsOnly)
    FString GameMapPath = TEXT("/Game/ThirdPerson/Lvl_ThirdPerson");

    void EvictAllPlayers();

    UPROPERTY(EditDefaultsOnly)
    FString MainMenuMap = TEXT("/Game/Maps/MainMenu");

protected:
    UFUNCTION()
    void OnSessionDestroyed();

private:
    bool bIsEvicting = false;
	
};
