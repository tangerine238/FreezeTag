// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyWidget.h"
#include "PC_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class FREEZETAG_API APC_Lobby : public APlayerController
{
	GENERATED_BODY()

public:
	void RequestSetReady(bool bReady);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

	virtual void BeginPlay() override;

private:
	UFUNCTION(Server, Reliable)
	void ServerSetReady(bool bReady);

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerName(const FString& NewName);


	ULobbyWidget* LobbyWidget = nullptr;
	
};
