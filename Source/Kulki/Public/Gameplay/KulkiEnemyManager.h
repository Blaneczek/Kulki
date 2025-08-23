// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KulkiEnemyManager.generated.h"

class AKulkiEnemyBaseCharacter;

USTRUCT(Blueprintable)
struct FSpawnDistanceThreshold
{
	GENERATED_BODY()

	FSpawnDistanceThreshold() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDistance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 0.f;
};

UCLASS()
class KULKI_API AKulkiEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AKulkiEnemyManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnDistanceThreshold> SpawnDistanceThresholds;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AKulkiEnemyBaseCharacter> EnemyClass;
	
private:
	void SpawnEnemies();
};
