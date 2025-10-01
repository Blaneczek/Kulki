// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KulkiMenuGameMode.generated.h"

class UKulkiMenuWidget;

/**
 * 
 */
UCLASS()
class KULKI_API AKulkiMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKulkiMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> Level01;
	
private:
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void ExitGame();

	UPROPERTY()
    TObjectPtr<UKulkiMenuWidget> MenuWidget;
	
};
