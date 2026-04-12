#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "MainMenu.generated.h"

class UMultiplayerSessionManager;
class UButton;
class UWidgetSwitcher;
class UEditableTextBox;

UCLASS()
class FREEZETAG_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SelectSession(int32 Index);

	void PopulateServerList();

private:

	// Subsystem ref
	UMultiplayerSessionManager *SessionManager;

	UPROPERTY(meta = (BindWidget))
	UButton *HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton *JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton *SettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton *QuitButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher *MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton *JoinConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UButton *JoinBackButton;

	UPROPERTY(meta = (BindWidget))
	UButton *SettingsBackButton;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox *ServerList;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *SearchStatusText;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PlayerNameInput;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UServerEntryWidget> ServerEntryWidgetClass;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnJoinConfirmClicked();

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnSessionCreated(bool bWasSuccessful);

	UFUNCTION()
	void OnSessionJoined(bool bWasSuccessful);

	UFUNCTION()
    void OnSessionsFound(bool bWasSuccessful);

	void SavePlayerName();

	static const int32 MainPanelIndex = 0;
	static const int32 JoinPanelIndex = 1;
	static const int32 SettingsPanelIndex = 2;

	int32 SelectedSessionIndex = -1;

	UServerEntryWidget* SelectedEntryWidget = nullptr;
};