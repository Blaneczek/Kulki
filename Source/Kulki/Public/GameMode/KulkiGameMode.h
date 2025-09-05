// Fill out your copyright notice in the Description page of Project Settings.

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
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WonGameWidgetClass;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UKulkiEnemyComponent> EnemyManager;
};
