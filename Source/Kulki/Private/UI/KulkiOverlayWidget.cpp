// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiOverlayWidget.h"

void UKulkiOverlayWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
