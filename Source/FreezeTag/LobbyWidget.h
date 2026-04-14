// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "LobbyWidget.generated.h"


class UScrollBox;
class UButton;
class UTextBlock;
class UWidget;

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

    UPROPERTY(meta = (BindWidget))
    UBorder* LoadingScreen;

    UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadingText;

private:
    bool bIsReady = false;

    UFUNCTION()
    void OnReadyClicked();

    UFUNCTION()
    void RefreshPlayerList();

    UFUNCTION()
    void ShowLoadingScreen();
};
