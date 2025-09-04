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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WonGameWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UKulkiEnemyComponent> EnemyManager;

protected:
	virtual void BeginPlay() override;
};
