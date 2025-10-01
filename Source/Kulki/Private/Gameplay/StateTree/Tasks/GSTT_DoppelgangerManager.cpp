// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


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
		Actor->OnCanMerge.BindWeakLambda(this, [this]()
		{
			BroadcastDelegate(Merge);	
		});
	}
	return EStateTreeRunStatus::Running;
	
}
