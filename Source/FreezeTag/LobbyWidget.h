// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UScrollBox;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class FREEZETAG_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* PlayerList;

    UPROPERTY(meta = (BindWidget))
    UButton* ReadyButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ReadyButtonText;

private:
    bool bIsReady = false;

    UFUNCTION()
    void OnReadyClicked();

    UFUNCTION()
    void RefreshPlayerList();
};
