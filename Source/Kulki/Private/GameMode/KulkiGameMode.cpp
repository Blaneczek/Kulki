// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KulkiGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Component/KulkiEnemyComponent.h"
#include "Kismet/GameplayStatics.h"

AKulkiGameMode::AKulkiGameMode()
{
	EnemyManager = CreateDefaultSubobject<UKulkiEnemyComponent>("EnemyManager");
}

void AKulkiGameMode::BeginPlay()
{
	Super::BeginPlay();

	FadeInCamera(4.f);
	
	EnemyManager->OnAllEatableEnemyKilled.AddUObject(this, &AKulkiGameMode::GameWon);
	EnemyManager->SpawnEnemies();
}

void AKulkiGameMode::FadeInCamera(float FadeOutTime)
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(1.f, 0.f,
		FadeOutTime, FLinearColor::Black, false, true);
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

	UGameplayStatics::SetGamePaused(this, true);
}


