// Fill out your copyright notice in the Description page of Project Settings.


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
		Actor->OnCheckIfBigger.AddWeakLambda(this, [this]()
		{
			if (PlayerPawn && !PlayerPawn->IsImmune())
			{
				BroadcastDelegate(CheckIfBigger);
			}		
		});
		Actor->OnBackToIdle.AddWeakLambda(this, [this]()
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
		auto a = PlayerPawn->OnImmunityActivation.GetAllocatedSize();
		UE_LOG(LogTemp, Warning, TEXT(": %llu"), a);
	}
}


