// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KulkiHUD.generated.h"

class AKulkiPlayerPawn;
class UKulkiWidgetController;
class UKulkiOverlayWidget;

/**
 * 
 */
UCLASS()
class KULKI_API AKulkiHUD : public AHUD
{
	GENERATED_BODY()

public:
	UKulkiWidgetController* GetWidgetController(AKulkiPlayerPawn* PlayerPawn);
	
	void InitOverlayWidget(AKulkiPlayerPawn* PlayerPawn);
	
private:
	UPROPERTY()
	TObjectPtr<UKulkiOverlayWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKulkiOverlayWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UKulkiWidgetController> WidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKulkiWidgetController> WidgetControllerClass;
};
