// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/KulkiEnemyManager.h"

#include "NavigationSystem.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Character/KulkiPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKulkiEnemyManager::AKulkiEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AKulkiEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	//SpawnEnemies_Debug();
	SpawnEnemies();
}

void AKulkiEnemyManager::SpawnEnemies()
{
	AKulkiPlayerCharacter* Player = Cast<AKulkiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player || !EnemyClass)
	{
		return;
	}
	const FVector PlayerLocation = Player->GetActorLocation();

	for (const auto& EnemyData : SpawnData)
	{
		for (const auto& DistanceRange : EnemyData.Value.DistanceRanges)
		{ 
			for (int32 i = 0; i < DistanceRange.NumberToSpawn; ++i)
			{
				const FVector RandomDirection = UKismetMathLibrary::RandomUnitVector().GetSafeNormal2D();
				const float RandomDistance = UKismetMathLibrary::RandomFloatInRange(DistanceRange.MinDistance, DistanceRange.MaxDistance);
				const FVector RandomLocationFromPlayer = PlayerLocation + FVector(
					RandomDirection.X * RandomDistance,
					RandomDirection.Y * RandomDistance,
					85.f);
				
				FNavLocation SpawnNavLocation;
				UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
				if (NavSystem && NavSystem->ProjectPointToNavigation(RandomLocationFromPlayer, SpawnNavLocation))
				{
					FTransform SpawnTransform;
					SpawnTransform.SetLocation(RandomLocationFromPlayer);
					AKulkiEnemyBaseCharacter* Enemy = GetWorld()->SpawnActorDeferred<AKulkiEnemyBaseCharacter>(
						EnemyClass,
						SpawnTransform,
						this,
						nullptr,
						ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				
					if (Enemy && EnemyData.Value.StrengthToDistanceCurve && EnemyData.Value.SpeedToDistanceCurve)
					{
						Enemy->Type = EnemyData.Key;
						const float Strength = EnemyData.Value.StrengthToDistanceCurve->GetFloatValue(RandomDistance);
						const float Speed = EnemyData.Value.SpeedToDistanceCurve->GetFloatValue(RandomDistance);
						Enemy->SetAttributesValue(Strength, Speed);
						UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);			
					}	
				}			
			}	
		}	
	}
}

void AKulkiEnemyManager::SpawnEnemies_Debug()
{
	AKulkiPlayerCharacter* Player = Cast<AKulkiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player)
	{
		return;
	}
	const FVector PlayerLocation = Player->GetActorLocation();
	
	for (int32 i = 0; i < 20; ++i)
	{
		if (EnemyClass)
		{		
			FNavLocation SpawnLocation;
			const bool bFoundRandomLocation = UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPoint(SpawnLocation);
			if (bFoundRandomLocation)
			{
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(FVector(SpawnLocation.Location.X, SpawnLocation.Location.Y, 85.f));
				AKulkiEnemyBaseCharacter* Enemy = GetWorld()->SpawnActorDeferred<AKulkiEnemyBaseCharacter>(EnemyClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				Enemy->Type = static_cast<EEnemyType>(UKismetMathLibrary::RandomIntegerInRange(1,3));
				Enemy->SetAttributesValue(UKismetMathLibrary::RandomIntegerInRange(10, 70), UKismetMathLibrary::RandomIntegerInRange(10, 50)); // TODO: change this
				UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);
				UE_LOG(LogTemp, Warning, TEXT("spawned enemy"));
			}		
		}	
	}
}



