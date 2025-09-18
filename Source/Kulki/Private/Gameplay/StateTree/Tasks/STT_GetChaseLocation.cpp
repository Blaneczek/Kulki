// Fill out your copyright notice in the Description page of Project Settings.


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
	if (Actor && Player)
	{
		// Add mesh (cylinder) radius 
		DistanceFromPlayerWithoutAvoidance = (Actor->GetAttributeSet()->GetStrength() * 5.f) + DistanceFromPlayerWithoutAvoidance;
		OutLocation = Player->GetActorLocation();
	}
	
	return Super::EnterState(Context, Transition);
}

EStateTreeRunStatus USTT_GetChaseLocation::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{	
	if (!Actor || !Player)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AvoidanceNeighbours = Actor->AvoidanceNeighbours;
	const FVector ActorLocation = Actor->GetActorLocation();
	const FVector PlayerLocation = Player->GetActorLocation();

	const float ActorDistanceFromPlayer = FVector::DistSquared(ActorLocation, PlayerLocation);
		
	if (AvoidanceNeighbours.IsEmpty() || ActorDistanceFromPlayer < FMath::Square(DistanceFromPlayerWithoutAvoidance))
	{
		// Early return, enemy is close enough to player or there are no neighbours
		OutLocation = PlayerLocation;
		
		return Super::Tick(Context, DeltaTime);
	}
	
	FVector Avoidance = FVector::ZeroVector;
	for (const auto& OtherEnemy : AvoidanceNeighbours)
	{
		if (AActor* OtherActor = OtherEnemy.Get())
		{
			Avoidance += (((ActorLocation - OtherActor->GetActorLocation()).GetSafeNormal()) * AvoidanceStrength);
		}
	}

	OutLocation = (PlayerLocation + Avoidance);

	return Super::Tick(Context, DeltaTime);
}
