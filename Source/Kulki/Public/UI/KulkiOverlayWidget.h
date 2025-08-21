// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KulkiOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class KULKI_API UKulkiOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();	
};
