// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KulkiMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

/**
 * 
 */
UCLASS()
class KULKI_API UKulkiMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UPROPERTY(BlueprintCallable)
	FOnButtonClicked OnStartGameClicked;

	UPROPERTY(BlueprintCallable)
	FOnButtonClicked OnExitGameClicked;

};
