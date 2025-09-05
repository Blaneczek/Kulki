// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/Data/KulkiEnemySpawnData.h"
#include "KulkiEnemyComponent.generated.h"

class AKulkiEnemyPawn;

DECLARE_MULTICAST_DELEGATE(FOnAllEatableEnemyKilled);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KULKI_API UKulkiEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKulkiEnemyComponent();
	
	void SpawnEnemies();
    
    /* Sets Enemies' State to IDLE and disables ability to chase the Player. */
    void StopChasingPlayer();
    
    /* Enables Enemies' ability to chase the Player. */
    void SetCanChasePlayer();
    
    /* Decrements number of eatable enemies and checks if it was the last one. */
    void EatableEnemyKilled();
	
	UPROPERTY(EditAnywhere, Category="Kulki")
	TSubclassOf<AKulkiEnemyPawn> EnemyClass;
	
	UPROPERTY(EditAnywhere, Category="Kulki")
	TObjectPtr<UKulkiEnemySpawnData> SpawnDataAsset;
	
	UPROPERTY(EditAnywhere, Category="Kulki")
	bool bNotSpawn_Debug;

	FOnAllEatableEnemyKilled OnAllEatableEnemyKilled;
	
protected:
	virtual void BeginPlay() override;

private:
	void BindDelegatesFromPlayer();

	FVector CalculateValidRandomLocation(const FVector& PlayerLocation, const FSpawnDistanceRange& DistanceRange, float& OutRandomDistance, bool& OutFoundValidLocation);

	void SpawnEnemy(const FVector& SpawnLocation, const TPair<EEnemyType, FSpawnEnemyData>& EnemyData, float RandomDistance, float DifficultyLevelScale);
	
	UPROPERTY()
	TArray<AKulkiEnemyPawn*> Enemies;

	uint32 NumberOfEatableEnemies = 0;
};
