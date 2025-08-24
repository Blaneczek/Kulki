// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/KulkiEnemyManager.h"

#include "NavigationSystem.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Character/KulkiPlayerCharacter.h"
#include "GameMode/KulkiGameMode.h"
#include "Gameplay/Data/KulkiEnemySpawnData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKulkiEnemyManager::AKulkiEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AKulkiEnemyManager::StopChasingPlayer()
{
	for (auto Enemy : Enemies)
	{
		if (IsValid(Enemy))
		{
			Enemy->SetState(EEnemyState::IDLE);
			Enemy->bCanChase = false;
		}
	}
}

void AKulkiEnemyManager::SetCanChasePlayer()
{
	for (auto Enemy : Enemies)
	{
		if (IsValid(Enemy))
		{
			Enemy->bCanChase = true;
		}
	}
}

void AKulkiEnemyManager::EatableEnemyKilled()
{
	NumberOfEatableEnemies--;
	if (NumberOfEatableEnemies <= 0)
	{
		if (AKulkiGameMode* GameMode = Cast<AKulkiGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode->GameWon();
		}		
	}
}

void AKulkiEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	if (bNotSpawn_Debug)
	{
		return;
	}
	SpawnEnemies();

	AKulkiPlayerCharacter* PlayerCharacter = Cast<AKulkiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		PlayerCharacter->OnImmunityActivation.BindUObject(this, &AKulkiEnemyManager::StopChasingPlayer);
		PlayerCharacter->OnImmunityDeactivation.BindUObject(this, &AKulkiEnemyManager::SetCanChasePlayer);
		PlayerCharacter->OnEnemyKilled.BindUObject(this, &AKulkiEnemyManager::EatableEnemyKilled);
	}
}

void AKulkiEnemyManager::SpawnEnemies()
{
	NumberOfEatableEnemies = 0;
	
	AKulkiPlayerCharacter* Player = Cast<AKulkiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player || !EnemyClass || !SpawnDataAsset)
	{
		return;
	}
	const FVector PlayerLocation = Player->GetActorLocation();

	//TODO: Get level
	
	for (const auto& EnemyData : SpawnDataAsset->SpawnData)
	{
		for (const auto& DistanceRange : EnemyData.Value.DistanceRanges)
		{ 
			for (uint32 i = 0; i < DistanceRange.NumberToSpawn; ++i)
			{			
				const FVector RandomDirection = UKismetMathLibrary::RandomUnitVector().GetSafeNormal2D();
				const float RandomDistance = FMath::RandRange(DistanceRange.MinDistance, DistanceRange.MaxDistance);
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
						Enemies.Add(Enemy);

						if (EnemyData.Key == EEnemyType::RED || EnemyData.Key == EEnemyType::YELLOW)
						{
							NumberOfEatableEnemies++;
						}
					}	
				}			
			}	
		}	
	}
}




