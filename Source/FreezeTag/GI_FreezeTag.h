// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_FreezeTag.generated.h"

/**
 * 
 */
UCLASS()
class FREEZETAG_API UGI_FreezeTag : public UGameInstance
{
	GENERATED_BODY()

public:
	FString LocalPlayerName = TEXT("Player");
	
};
