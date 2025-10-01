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
	
	BindEnemyDelegates();
	BindPlayerDelegates();
	
	return EStateTreeRunStatus::Running;
}


void UGSTT_EnemyManager::BindEnemyDelegates()
{
	if (Actor)
	{
		Actor->OnCheckIfBigger.BindWeakLambda(this, [this]()
		{
			if (PlayerPawn && !PlayerPawn->IsImmune())
			{
				BroadcastDelegate(CheckIfBigger);			
			}		
		});
		Actor->OnBackToIdle.BindWeakLambda(this, [this]()
		{
			BroadcastDelegate(BackToIdle);
		});
	}	
}

void UGSTT_EnemyManager::BindPlayerDelegates()
{
	if (PlayerPawn && Actor)
	{
		PlayerPawn->OnImmunityActivation.AddWeakLambda(this, [this]()
		{
			if (!Actor->bIdleState)
			{
				BroadcastDelegate(BackToIdle);
			}			
		});
		PlayerPawn->OnImmunityDeactivation.AddWeakLambda(this, [this]()
		{
			if (!Actor->bIdleState)
			{
				BroadcastDelegate(CheckIfBigger);
			}
		});
		PlayerPawn->OnStrengthChanged.AddWeakLambda(this, [this]()
		{
			if (!Actor->bIdleState && !PlayerPawn->IsImmune())
			{
				BroadcastDelegate(CheckIfBigger);
			}	
		});
	}
}


