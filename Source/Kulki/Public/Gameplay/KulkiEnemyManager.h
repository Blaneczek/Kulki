// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "GameFramework/Actor.h"
#include "KulkiEnemyManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnDistanceRange
{
	GENERATED_BODY()

	FSpawnDistanceRange() {}

	UPROPERTY(EditAnywhere)
	float MinDistance = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 0.f;
	
	UPROPERTY(EditAnywhere)
	uint32 NumberToSpawn = 0.f;
};


USTRUCT(BlueprintType)
struct FSpawnEnemyData
{
	GENERATED_BODY()

	FSpawnEnemyData() {}
	
	UPROPERTY(EditAnywhere)
	TArray<FSpawnDistanceRange> DistanceRanges;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> StrengthToDistanceCurve = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> SpeedToDistanceCurve = nullptr;
};

UCLASS()
class KULKI_API AKulkiEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AKulkiEnemyManager();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AKulkiEnemyBaseCharacter> EnemyClass;

	UPROPERTY(EditAnywhere)
	TMap<EEnemyType, FSpawnEnemyData> SpawnData;

	UPROPERTY(EditAnywhere)
	bool bNotSpawn_Debug;
	
protected:
	virtual void BeginPlay() override;

private:
	void SpawnEnemies();
};
