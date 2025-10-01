// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "GameMode/KulkiMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/KulkiMenuWidget.h"

void AKulkiMenuGameMode::BeginPlay()
{
    AGameModeBase::BeginPlay();

    checkf(MenuWidgetClass, TEXT("AKulkiMenuGameMode | MenuWidgetClass is not set"));
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
    if (!Level01.IsNull())
    {
        UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level01);
    }
}

void AKulkiMenuGameMode::ExitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
