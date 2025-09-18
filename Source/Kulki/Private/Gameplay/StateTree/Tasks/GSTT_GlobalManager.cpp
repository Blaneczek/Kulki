// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StateTree/Tasks/GSTT_GlobalManager.h"

#include "Character/KulkiEnemyPawn.h"
#include "Character/KulkiPlayerPawn.h"
#include "Slate/SGameLayerManager.h"

UGSTT_GlobalManager::UGSTT_GlobalManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = false;
}

EStateTreeRunStatus UGSTT_GlobalManager::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	
	BindEnemyDelegates();
	BindPlayerDelegates();
	
	return EStateTreeRunStatus::Running;
}

void UGSTT_GlobalManager::BindEnemyDelegates()
{
	if (Actor)
	{
		Actor->OnCheckIfBigger.AddLambda([this]()
		{
			if (PlayerPawn && !PlayerPawn->IsImmune())
			{
				BroadcastDelegate(CheckIfBigger);
			}		
		});
		Actor->OnBackToIdle.AddLambda([this]()
		{
			BroadcastDelegate(BackToIdle);
		});
	}
}

void UGSTT_GlobalManager::BindPlayerDelegates()
{
	if (PlayerPawn && Actor)
	{
		PlayerPawn->OnImmunityActivation.AddLambda([this]()
		{
			if (!Actor->bIdleState)
			{
				BroadcastDelegate(BackToIdle);
			}			
		});
		PlayerPawn->OnImmunityDeactivation.AddLambda([this]()
		{
			if (!Actor->bIdleState)
			{
				BroadcastDelegate(CheckIfBigger);
			}
		});
	}
}


