// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StateTree/Tasks/GSTT_DoppelgangerManager.h"

#include "Character/KulkiDoppelgangerPawn.h"

UGSTT_DoppelgangerManager::UGSTT_DoppelgangerManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EStateTreeRunStatus UGSTT_DoppelgangerManager::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (Actor)
	{
		Actor->OnCanMerge.AddWeakLambda(this, [this]()
		{
			BroadcastDelegate(Merge);	
		});
	}
	return EStateTreeRunStatus::Running;
	
}
