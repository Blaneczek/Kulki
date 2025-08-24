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
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AKulkiEnemyBaseCharacter> EnemyClass;
	
	UPROPERTY(EditAnywhere, Category = "Kulki")
	TObjectPtr<UKulkiEnemySpawnData> SpawnDataAsset;
	
	UPROPERTY(EditAnywhere)
	bool bNotSpawn_Debug;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnEnemies();
	
	void StopChasingPlayer();
	void SetCanChasePlayer();

	void EatableEnemyKilled();
	
	UPROPERTY()
	TArray<AKulkiEnemyBaseCharacter*> Enemies;

	uint32 NumberOfEatableEnemies = 0;
};
