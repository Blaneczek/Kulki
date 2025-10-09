// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/Tasks/GSTT_EnemyManager.h"
#include "Character/KulkiEnemyPawn.h"
#include "Character/KulkiPlayerPawn.h"

UGSTT_EnemyManager::UGSTT_EnemyManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = false;
}

EStateTreeRunStatus UGSTT_EnemyManager::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (!PlayerPawn || !Actor)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	BindEnemyDelegates();
	BindPlayerDelegates();
	
	return EStateTreeRunStatus::Running;
}


void UGSTT_EnemyManager::BindEnemyDelegates()
{
	Actor->OnCheckIfBigger.BindWeakLambda(this, [this]()
	{
		if (IsValid(Actor) && PlayerPawn && !PlayerPawn->IsImmune())
		{
			BroadcastDelegate(CheckIfBigger);			
		}		
	});
	Actor->OnBackToIdle.BindWeakLambda(this, [this]()
	{
		if (IsValid(Actor))
		{
			BroadcastDelegate(BackToIdle);
		}		
	});	
}

void UGSTT_EnemyManager::BindPlayerDelegates()
{
	PlayerPawn->OnImmunityActivation.AddWeakLambda(this, [this]()
	{
		if (IsValid(Actor) && !Actor->bIdleState)
		{
			BroadcastDelegate(BackToIdle);
		}			
	});
	PlayerPawn->OnImmunityDeactivation.AddWeakLambda(this, [this]()
	{
		if (IsValid(Actor) && !Actor->bIdleState)
		{
			BroadcastDelegate(CheckIfBigger);
		}
	});
	PlayerPawn->OnStrengthChanged.AddWeakLambda(this, [this]()
	{
		if (IsValid(Actor) && !Actor->bIdleState && !PlayerPawn->IsImmune())
		{
			BroadcastDelegate(CheckIfBigger);
		}	
	});
}


