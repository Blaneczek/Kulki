// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KulkiGameMode.generated.h"

class UKulkiEnemyComponent;
/**
 * 
 */
UCLASS()
class KULKI_API AKulkiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKulkiGameMode();
	
	void ResetGame();
	void GameWon();

	UKulkiEnemyComponent* GetEnemyManager() const { return EnemyManager; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Kulki")
    TSubclassOf<UUserWidget> WonGameWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
    TObjectPtr<UKulkiEnemyComponent> EnemyManager;
};
