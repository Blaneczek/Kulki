// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/Tasks/STT_GetChaseLocation.h"
#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"
#include "Character/KulkiEnemyPawn.h"

USTT_GetChaseLocation::USTT_GetChaseLocation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = true;
	OutLocation = FVector::ZeroVector;
	AvoidanceStrength = 0.f;
	DistanceFromPlayerWithoutAvoidance = 0.f;
}

EStateTreeRunStatus USTT_GetChaseLocation::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	if (Actor && Player)
	{
		// Add mesh (cylinder) radius 
		DistanceFromPlayerWithoutAvoidance = (Actor->GetAttributeSet()->GetStrength() * 5.f) + DistanceFromPlayerWithoutAvoidance;
		OutLocation = Player->GetActorLocation();
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_GetChaseLocation::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{	
	OutLocation = GetAvoidanceLocation();

	return Super::Tick(Context, DeltaTime);
}

FVector USTT_GetChaseLocation::GetAvoidanceLocation()
{
	if (!Actor || !Player)
	{
		FinishTask(false);
		return FVector::ZeroVector;
	}
	
	AvoidanceNeighbors = Actor->AvoidanceNeighbors;
	const FVector ActorLocation = Actor->GetActorLocation();
	const FVector PlayerLocation = Player->GetActorLocation();

	const float ActorDistanceFromPlayer = FVector::DistSquared(ActorLocation, PlayerLocation);	
	if (AvoidanceNeighbors.IsEmpty() || ActorDistanceFromPlayer < FMath::Square(DistanceFromPlayerWithoutAvoidance))
	{
		// Early return, enemy is close enough to player or there are no neighbors
		return PlayerLocation;
	}
	
	FVector Avoidance = FVector::ZeroVector;
	for (const auto& OtherEnemy : AvoidanceNeighbors)
	{
		if (const AActor* OtherActor = OtherEnemy.Get())
		{
			Avoidance += (((ActorLocation - OtherActor->GetActorLocation()).GetSafeNormal()) * AvoidanceStrength);
		}
	}

	return PlayerLocation + Avoidance;
}


