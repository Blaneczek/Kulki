// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiHUD.h"
#include "UI/KulkiOverlayWidget.h"
#include "UI/KulkiWidgetController.h"

UKulkiWidgetController* AKulkiHUD::GetWidgetController(AKulkiPlayerCharacter* PlayerCharacter)
{
	// Create if it is first time
	if (!WidgetController)
	{
		WidgetController = NewObject<UKulkiWidgetController>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(PlayerCharacter);
	}
	return WidgetController;
}

void AKulkiHUD::InitOverlayWidget(AKulkiPlayerCharacter* PlayerCharacter)
{
	checkf(OverlayWidgetClass, TEXT("AKulkiHUD | OverlayWidgetClass is not set"));
	checkf(WidgetControllerClass, TEXT("AKulkiHUD | WidgetControllerClass is not set"));
	
	OverlayWidget = CreateWidget<UKulkiOverlayWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget->SetWidgetController(GetWidgetController(PlayerCharacter));
	
	GetWidgetController(PlayerCharacter)->InitAttributesValue();
	GetWidgetController(PlayerCharacter)->BindCallbacks();
		
	OverlayWidget->AddToViewport();
}
