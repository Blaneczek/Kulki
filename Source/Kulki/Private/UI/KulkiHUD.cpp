// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiHUD.h"
#include "Character/KulkiPlayerPawn.h"
#include "UI/KulkiOverlayWidget.h"
#include "UI/KulkiWidgetController.h"

UKulkiWidgetController* AKulkiHUD::GetWidgetController(const FWidgetControllerParams& WCParams)
{
	// Create if it is first time
	if (!WidgetController)
	{
		WidgetController = NewObject<UKulkiWidgetController>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(WCParams);
		WidgetController->BindCallbacks();
	}
	return WidgetController;
}

void AKulkiHUD::InitOverlayWidget(APlayerController* PlayerController, AKulkiPlayerPawn* PlayerPawn, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("AKulkiHUD | OverlayWidgetClass is not set"));
	checkf(WidgetControllerClass, TEXT("AKulkiHUD | WidgetControllerClass is not set"));
	
	OverlayWidget = CreateWidget<UKulkiOverlayWidget>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerPawn, ASC, AS);
	UKulkiWidgetController* Controller = GetWidgetController(WidgetControllerParams);
	
    OverlayWidget->SetWidgetController(Controller);
	Controller->BroadcastInitialValues();
		
	OverlayWidget->AddToViewport();
}
