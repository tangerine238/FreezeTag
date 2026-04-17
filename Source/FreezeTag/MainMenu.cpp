#include "MainMenu.h"
#include "MultiplayerSessionManager.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "GI_FreezeTag.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    SessionManager = GetGameInstance()->GetSubsystem<UMultiplayerSessionManager>();
    if (SessionManager)
    {
        SessionManager->DestroySession();
        SessionManager->OnCreateSessionComplete.AddDynamic(this, &UMainMenu::OnSessionCreated);
        SessionManager->OnJoinSessionComplete.AddDynamic(this, &UMainMenu::OnSessionJoined);
        SessionManager->OnFindSessionsComplete.AddDynamic(this, &UMainMenu::OnSessionsFound);
    }

    // Bind buttons
    if (HostButton)
        HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);
    if (JoinButton)
        JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClicked);
    if (SettingsButton)
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::OnSettingsClicked);
    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClicked);

    if (JoinConfirmButton)
        JoinConfirmButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinConfirmClicked);
    if (JoinBackButton)
        JoinBackButton->OnClicked.AddDynamic(this, &UMainMenu::OnBackClicked);
    if (SettingsBackButton)
        SettingsBackButton->OnClicked.AddDynamic(this, &UMainMenu::OnBackClicked);
}

void UMainMenu::OnHostClicked()
{
    SavePlayerName();
    if (SessionManager)
        SessionManager->CreateSession(4, FString("FreezeTag"));

    UE_LOG(LogTemp, Warning, TEXT("Host clicked"));
    if (SessionManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session manager found, creating session"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Session manager is NULL"));
    }
}

void UMainMenu::OnJoinClicked()
{
    if (MenuSwitcher)
        MenuSwitcher->SetActiveWidgetIndex(JoinPanelIndex);

    if (ServerList)
        ServerList->ClearChildren();
    if (SearchStatusText)
        SearchStatusText->SetText(FText::FromString("Searching..."));

    if (SessionManager)
        SessionManager->FindSessions(10000);
}

void UMainMenu::OnSettingsClicked()
{
    if (MenuSwitcher)
        MenuSwitcher->SetActiveWidgetIndex(SettingsPanelIndex);
}

void UMainMenu::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UMainMenu::OnJoinConfirmClicked()
{
    SavePlayerName();

    if (SelectedSessionIndex < 0)
        return;
    if (!SessionManager)
        return;
    if (!SessionManager->LastSearchResults.IsValidIndex(SelectedSessionIndex))
        return;

    SessionManager->JoinSession(SessionManager->LastSearchResults[SelectedSessionIndex]);
}

void UMainMenu::SelectSession(int32 Index)
{
    if (SelectedEntryWidget)
        SelectedEntryWidget->SetSelected(false);

    SelectedSessionIndex = Index;

    if (ServerList)
    {
        UWidget *Child = ServerList->GetChildAt(Index);
        SelectedEntryWidget = Cast<UServerEntryWidget>(Child);
        if (SelectedEntryWidget)
            SelectedEntryWidget->SetSelected(true);
    }
}

void UMainMenu::OnBackClicked()
{
    if (MenuSwitcher)
        MenuSwitcher->SetActiveWidgetIndex(MainPanelIndex);
}

void UMainMenu::OnSessionCreated(bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        GetWorld()->ServerTravel(FString("/Game/Level/Lobby?listen"));
    }
}

void UMainMenu::OnSessionJoined(bool bWasSuccessful)
{
    if (!bWasSuccessful)
    {
    }
}

void UMainMenu::PopulateServerList()
{

    if (!ServerList || !ServerEntryWidgetClass)
        return;
    ServerList->ClearChildren();

    const TArray<FOnlineSessionSearchResult> &Results = SessionManager->LastSearchResults;

    if (Results.Num() == 0)
    {
        if (SearchStatusText)
            SearchStatusText->SetText(FText::FromString("No sessions found."));
        return;
    }

    if (SearchStatusText)
        SearchStatusText->SetText(FText::FromString(""));

    TSet<FString> SeenSessions;

    for (int32 i = 0; i < Results.Num(); i++)
    {
        const FOnlineSessionSearchResult &Result = Results[i];

        FString SessionId = Result.Session.GetSessionIdStr();
        if (SeenSessions.Contains(SessionId))
            continue;
        SeenSessions.Add(SessionId);

        UServerEntryWidget *Entry = CreateWidget<UServerEntryWidget>(this, ServerEntryWidgetClass);
        if (!Entry)
            continue;

        FString Name;
        Result.Session.SessionSettings.Get(FName("ServerName"), Name);
        if (Name.IsEmpty())
            Name = Result.Session.OwningUserName;
        int32 MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
        int32 CurrentPlayers = MaxPlayers - Result.Session.NumOpenPublicConnections;

        Entry->Setup(this, i, Name, CurrentPlayers, MaxPlayers);
        ServerList->AddChild(Entry);
    }
}

void UMainMenu::OnSessionsFound(bool bWasSuccessful)
{
    PopulateServerList();
}

void UMainMenu::SavePlayerName()
{
    if (!PlayerNameInput)
        return;
    FString Name = PlayerNameInput->GetText().ToString().TrimStartAndEnd();
    if (Name.IsEmpty())
        Name = TEXT("Player");

    if (UGI_FreezeTag *GI = GetGameInstance<UGI_FreezeTag>())
        GI->LocalPlayerName = Name;
}