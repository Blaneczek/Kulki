// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StateTree/Tasks/STT_MergeDoppelganger.h"

#include "Character/KulkiDoppelgangerPawn.h"
#include "Gameplay/StateTree/KulkiAIController.h"

USTT_MergeDoppelganger::USTT_MergeDoppelganger(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = false;
}

EStateTreeRunStatus USTT_MergeDoppelganger::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	
	if (!Actor)
	{
		return EStateTreeRunStatus::Failed;
	}

	Actor->MergeWithOwner();	
	return EStateTreeRunStatus::Succeeded;
}




