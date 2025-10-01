// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

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
