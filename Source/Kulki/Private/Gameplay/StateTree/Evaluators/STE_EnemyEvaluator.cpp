// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/Evaluators/STE_EnemyEvaluator.h"
#include "Character/KulkiEnemyPawn.h"

void USTE_EnemyEvaluator::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);

	bHasTick = false;
	
	if (Actor)
	{
		EnemyAttributeSet = Actor->GetAttributeSet();
		SpawnLocation = Actor->GetActorLocation();
	}
}

void USTE_EnemyEvaluator::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);

	if (Actor)
	{
		Actor->OnCheckIfBigger.RemoveAll(this);
		Actor->OnBackToIdle.RemoveAll(this);
	}
	
	Actor = nullptr;
	EnemyAttributeSet = nullptr;
	SpawnLocation = FVector::ZeroVector;
}


