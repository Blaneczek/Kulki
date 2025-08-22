// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/KulkiEnemyController.h"

#include "NavigationSystem.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Character/KulkiPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKulkiEnemyController::AKulkiEnemyController()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AKulkiEnemyController::BeginPlay()
{
	Super::BeginPlay();

	SpawnEnemies();
}

void AKulkiEnemyController::SpawnEnemies()
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
				AKulkiEnemyBaseCharacter* Enemy = GetWorld()->SpawnActorDeferred<AKulkiEnemyBaseCharacter>(EnemyClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				Enemy->Type = static_cast<EEnemyType>(UKismetMathLibrary::RandomIntegerInRange(1,3));
				Enemy->DebugStrength = UKismetMathLibrary::RandomIntegerInRange(10, 70);
				Enemy->DebugSpeed = UKismetMathLibrary::RandomIntegerInRange(10, 50);
				UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);
				UE_LOG(LogTemp, Warning, TEXT("spawned enemy"));
			}		
		}
		
	}
}



