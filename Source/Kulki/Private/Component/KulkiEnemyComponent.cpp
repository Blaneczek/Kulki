// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Component/KulkiEnemyComponent.h"
#include "NavigationSystem.h"
#include "Character/KulkiPlayerPawn.h"
#include "GameInstance/KulkiGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UKulkiEnemyComponent::UKulkiEnemyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bNotSpawn_Debug = false;
}

void UKulkiEnemyComponent::BeginPlay()
{
	Super::BeginPlay();

	BindDelegatesFromPlayer();
}

void UKulkiEnemyComponent::BindDelegatesFromPlayer()
{
	if (AKulkiPlayerPawn* PlayerPawn = Cast<AKulkiPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		PlayerPawn->OnEatableEnemyKilled.BindUObject(this, &UKulkiEnemyComponent::EatableEnemyKilled);
	}
}

void UKulkiEnemyComponent::SpawnEnemies()
{
	checkf(SpawnDataAsset, TEXT("UKulkiEnemyComponent::SpawnEnemies | SpawnDataAsset is not valid"));
	
#if WITH_EDITOR
	if (bNotSpawn_Debug)
	{
		return;
	}
#endif
	
	NumberOfEatableEnemies = 0;
	const AKulkiPlayerPawn* Player = Cast<AKulkiPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player)
	{
		return;
	}
	
	// Get chosen difficulty level
	int32 DifficultyLevel = 1;
	float DifficultyLevelScale = 1.f;
	if (const UKulkiGameInstance* GameInstance = Cast<UKulkiGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		DifficultyLevel = GameInstance->DifficultyLevel;
	}
	if (SpawnDataAsset->LevelScales.Contains(DifficultyLevel))
	{
		DifficultyLevelScale = *SpawnDataAsset->LevelScales.Find(DifficultyLevel);
	}

	bool bFoundValidLocation = false;
	float DistanceFromPlayer = 0.f;
	const FVector PlayerLocation = Player->GetActorLocation();
	
	for (const auto& EnemyData : SpawnDataAsset->SpawnData)
	{
		for (const auto& DistanceRange : EnemyData.Value.DistanceRanges)
		{ 
			for (uint32 i = 0; i < DistanceRange.NumberToSpawn; ++i)
			{
				const FVector SpawnLocation = CalculateValidRandomLocation(PlayerLocation, DistanceRange, DistanceFromPlayer, bFoundValidLocation);
				if (bFoundValidLocation)
				{
					SpawnEnemy(SpawnLocation, EnemyData, DistanceFromPlayer, DifficultyLevelScale);	
				}					
			}	
		}	
	}
}

FVector UKulkiEnemyComponent::CalculateValidRandomLocation(const FVector& PlayerLocation, const FSpawnDistanceRange& DistanceRange, float& OutDistanceFromPlayer, bool& OutFoundValidLocation)
{
	const FVector RandomDirection = UKismetMathLibrary::RandomUnitVector().GetSafeNormal2D();	
	OutDistanceFromPlayer = FMath::RandRange(DistanceRange.MinDistance, DistanceRange.MaxDistance);
	
	const FVector RandomLocationFromPlayer = PlayerLocation + FVector(RandomDirection.X * OutDistanceFromPlayer, RandomDirection.Y * OutDistanceFromPlayer, 42.f);

	if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation SpawnNavLocation;
		NavSystem->ProjectPointToNavigation(RandomLocationFromPlayer, SpawnNavLocation) ? OutFoundValidLocation = true : OutFoundValidLocation = false;
	}

	return FVector(RandomLocationFromPlayer.X, RandomLocationFromPlayer.Y, 42.f);
}

void UKulkiEnemyComponent::SpawnEnemy(const FVector& SpawnLocation, const TPair<EEnemyType, FSpawnEnemyData>& EnemyData, float DistanceFromPlayer, float DifficultyLevelScale)
{
	checkf(EnemyClass, TEXT("UKulkiEnemyComponent::SpawnEnemy | EnemyClass is not valid"));
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	AKulkiEnemyPawn* Enemy = GetWorld()->SpawnActorDeferred<AKulkiEnemyPawn>(
		EnemyClass,
		SpawnTransform,
		GetOwner(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
                    				
	if (IsValid(Enemy) && EnemyData.Value.StrengthToDistanceCurve && EnemyData.Value.SpeedToDistanceCurve)
	{
		Enemy->Type = EnemyData.Key;
		Enemy->SetOverlapGameplayEffectClass(*OverlapGameplayEffectClasses.Find(EnemyData.Key));
		const float Strength = EnemyData.Value.StrengthToDistanceCurve->GetFloatValue(DistanceFromPlayer) * DifficultyLevelScale;
		const float Speed = EnemyData.Value.SpeedToDistanceCurve->GetFloatValue(DistanceFromPlayer) * DifficultyLevelScale;	
		UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);	
		Enemy->InitSpawn(Strength, Speed);

		Enemies.Add(Enemy);		
		if (EnemyData.Key != EEnemyType::PURPLE)
		{
			NumberOfEatableEnemies++;
		}
	}				
}

void UKulkiEnemyComponent::EatableEnemyKilled()
{
	NumberOfEatableEnemies--;
	if (NumberOfEatableEnemies <= 0)
	{
		OnAllEatableEnemyKilled.ExecuteIfBound();		
	}
}


