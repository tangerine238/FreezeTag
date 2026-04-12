// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerEntryWidget.generated.h"

class UTextBlock;
class UButton;
class UBorder;

/**
 *
 */
UCLASS()
class FREEZETAG_API UServerEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(class UMainMenu *ParentMenu, int32 InSessionIndex, const FString &SessionName, int32 CurrentPlayers, int32 MaxPlayers);

	void SetSelected(bool bSelected);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock *SessionNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *PlayerCountText;

	UPROPERTY(meta = (BindWidget))
	UBorder* Background;

	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UMainMenu *ParentMenu;
	int32 SessionIndex;
	bool bIsSelected = false;

	const FLinearColor NormalColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.8f);
	const FLinearColor HoverColor = FLinearColor(0.2f, 0.2f, 0.2f, 0.9f);
	const FLinearColor SelectedColor = FLinearColor(0.0f, 0.4f, 0.8f, 1.0f);
};
