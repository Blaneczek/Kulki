// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "GameFramework/Actor.h"
#include "KulkiEnemyManager.generated.h"

class UKulkiEnemySpawnData;

UCLASS()
class KULKI_API AKulkiEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AKulkiEnemyManager();
	
	UPROPERTY(EditAnywhere, Category="Kulki")
	TSubclassOf<AKulkiEnemyBaseCharacter> EnemyClass;
	
	UPROPERTY(EditAnywhere, Category="Kulki")
	TObjectPtr<UKulkiEnemySpawnData> SpawnDataAsset;
	
	UPROPERTY(EditAnywhere, Category="Kulki")
	bool bNotSpawn_Debug;

protected:
	virtual void BeginPlay() override;

private:
	void BindDelegatesFromPlayer();
	
	void SpawnEnemies();

	/* Sets Enemies' State to IDLE and disables ability to chase the Player. */
	void StopChasingPlayer();

	/* Enables Enemies' ability to chase the Player. */
	void SetCanChasePlayer();

	/* Decrements number of eatable enemies and checks if it was the last one. */
	void EatableEnemyKilled();
	
	UPROPERTY()
	TArray<AKulkiEnemyBaseCharacter*> Enemies;

	uint32 NumberOfEatableEnemies = 0;
};
