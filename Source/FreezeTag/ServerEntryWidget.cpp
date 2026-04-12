// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerEntryWidget.h"
#include "MainMenu.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UServerEntryWidget::Setup(UMainMenu *InParentMenu, int32 InSessionIndex,
                               const FString &SessionName, int32 CurrentPlayers, int32 MaxPlayers)
{
    ParentMenu = InParentMenu;
    SessionIndex = InSessionIndex;

    if (SessionNameText)
        SessionNameText->SetText(FText::FromString(SessionName));

    if (PlayerCountText)
        PlayerCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentPlayers, MaxPlayers)));

    if (Background)
        Background->SetBrushColor(NormalColor);

}

void UServerEntryWidget::SetSelected(bool bSelected)
{
    bIsSelected = bSelected;
    if (Background)
        Background->SetBrushColor(bIsSelected ? SelectedColor : NormalColor);
}

FReply UServerEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent)
{
    if (ParentMenu)
        ParentMenu->SelectSession(SessionIndex);

    return FReply::Handled();
}

void UServerEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent)
{
    if (!bIsSelected && Background)
        Background->SetBrushColor(HoverColor);
}

void UServerEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    if (!bIsSelected && Background)
        Background->SetBrushColor(NormalColor);
}