// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "UI/KulkiOverlayWidget.h"

void UKulkiOverlayWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
