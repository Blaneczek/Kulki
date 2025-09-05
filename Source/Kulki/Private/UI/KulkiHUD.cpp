// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiHUD.h"

#include "Character/KulkiPlayerPawn.h"
#include "UI/KulkiOverlayWidget.h"
#include "UI/KulkiWidgetController.h"

UKulkiWidgetController* AKulkiHUD::GetWidgetController(AKulkiPlayerPawn* PlayerPawn)
{
	// Create if it is first time
	if (!WidgetController)
	{
		WidgetController = NewObject<UKulkiWidgetController>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(PlayerPawn);
	}
	return WidgetController;
}

void AKulkiHUD::InitOverlayWidget(AKulkiPlayerPawn* PlayerPawn)
{
	checkf(OverlayWidgetClass, TEXT("AKulkiHUD | OverlayWidgetClass is not set"));
	checkf(WidgetControllerClass, TEXT("AKulkiHUD | WidgetControllerClass is not set"));
	
	OverlayWidget = CreateWidget<UKulkiOverlayWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget->SetWidgetController(GetWidgetController(PlayerPawn));
	
	GetWidgetController(PlayerPawn)->InitAttributesValue();
	GetWidgetController(PlayerPawn)->BindCallbacks();
		
	OverlayWidget->AddToViewport();
}
