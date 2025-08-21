// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiHUD.h"
#include "UI/KulkiOverlayWidget.h"
#include "UI/KulkiWidgetController.h"

UKulkiWidgetController* AKulkiHUD::GetWidgetController(AKulkiPlayerCharacter* PlayerCharacter)
{
	if (!WidgetController)
	{
		WidgetController = NewObject<UKulkiWidgetController>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(PlayerCharacter);
	}
	return WidgetController;
}

void AKulkiHUD::InitOverlayWidget(AKulkiPlayerCharacter* PlayerCharacter)
{
	check(OverlayWidgetClass);
	check(WidgetControllerClass);

	OverlayWidget = CreateWidget<UKulkiOverlayWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetWidgetController(GetWidgetController(PlayerCharacter));
	WidgetController->InitAttributesValue();
	WidgetController->BindCallbacks();
	
	OverlayWidget->AddToViewport();
}
