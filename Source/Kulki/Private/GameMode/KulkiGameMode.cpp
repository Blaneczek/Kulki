// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KulkiGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AKulkiGameMode::AKulkiGameMode()
{
	
}

void AKulkiGameMode::PauseGame(bool bPauseGame)
{
	UGameplayStatics::SetGamePaused(this, bPauseGame);
}

void AKulkiGameMode::ResetGame()
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.f,
		1.f, 1.f, FColor::Black, false, true);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
	{
		const FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
        UGameplayStatics::OpenLevel(this, CurrentLevelName);
	});
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer, TimerDelegate, 1.f, false);
}

void AKulkiGameMode::GameWon()
{
	checkf(WonGameWidgetClass, TEXT("AKulkiGameMode | WonGameWidgetClass is not set"));

	if (UUserWidget* WonGameWidget = CreateWidget<UUserWidget>(GetWorld(), WonGameWidgetClass))
	{
		WonGameWidget->AddToViewport();
	}

	PauseGame(true);
}
