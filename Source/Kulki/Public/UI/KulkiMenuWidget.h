// Fill out your copyright notice in the Description page of Project Settings.

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
