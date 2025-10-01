// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/Tasks/STT_CheckIfBigger.h"
#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"
#include "Character/KulkiEnemyPawn.h"

USTT_CheckIfBigger::USTT_CheckIfBigger(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = false;
}

EStateTreeRunStatus USTT_CheckIfBigger::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	
	if (!PlayerAttributeSet || !EnemyAttributeSet || !Actor)
	{
		return EStateTreeRunStatus::Failed;
	}

	if (EnemyAttributeSet->GetStrength() > PlayerAttributeSet->GetStrength())
	{
		Actor->ResetNeighbours();
		BroadcastDelegate(StartChase);	
	}
	else
	{
		BroadcastDelegate(StartEscape);
	}

	return EStateTreeRunStatus::Succeeded;
}
