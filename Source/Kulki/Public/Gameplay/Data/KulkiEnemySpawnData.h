// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Engine/DataAsset.h"
#include "KulkiEnemySpawnData.generated.h"

USTRUCT(BlueprintType)
struct FSpawnDistanceRange
{
	GENERATED_BODY()

	FSpawnDistanceRange() {}

	/* Enemies will spawn between MinDistance and MaxDistance from Player position. */
	UPROPERTY(EditAnywhere, meta = (UIMin = "0.0", ClampMin = "0.0"))
	float MinDistance = 0.f;
	
	/* Enemies will spawn between MinDistance and MaxDistance from Player position. Must be >= than MinDistance. */
	UPROPERTY(EditAnywhere, meta = (UIMin = "0.0", ClampMin = "0.0"))
	float MaxDistance = 0.f;

	/* Number of enemies that will spawn within this distance range. */
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

	/* Curve to set Enemies' Strength scalability based on their distance from Player when they spawn. */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> StrengthToDistanceCurve = nullptr;
	
	/* Curve to set Enemies' Speed scalability based on their distance from Player when they spawn. */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> SpeedToDistanceCurve = nullptr;
};

/**
 * 
 */
UCLASS(Blueprintable)
class KULKI_API UKulkiEnemySpawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	UKulkiEnemySpawnData();
	
	UPROPERTY(EditAnywhere)
	TMap<EEnemyType, FSpawnEnemyData> SpawnData;

	/*
	 * Enemies' Strength and Speed are scaled base on difficulty level.
	 * Key - Difficulty level
	 * Value - Multiplier
	 */
	UPROPERTY(EditAnywhere)
	TMap<uint32, float> LevelScales;
};
