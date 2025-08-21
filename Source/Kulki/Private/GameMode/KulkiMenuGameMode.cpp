// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KulkiMenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UI/KulkiMenuWidget.h"

void AKulkiMenuGameMode::BeginPlay()
{
    AGameModeBase::BeginPlay();

    check(MenuWidgetClass);
    MenuWidget = CreateWidget<UKulkiMenuWidget>(GetWorld(), MenuWidgetClass);

    if (IsValid(MenuWidget))
    {
        MenuWidget->OnStartGameClicked.AddDynamic(this, &AKulkiMenuGameMode::StartGame);
        MenuWidget->OnExitGameClicked.AddDynamic(this, &AKulkiMenuGameMode::ExitGame);
        MenuWidget->AddToViewport();
    }

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PC->SetShowMouseCursor(true);
        FInputModeUIOnly InputModeData;
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputModeData);
    }
}

void AKulkiMenuGameMode::StartGame()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level01);
}

void AKulkiMenuGameMode::ExitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, true);
}
