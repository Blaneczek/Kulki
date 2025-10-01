// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/Tasks/STT_StopChasingAfterTime.h"
#include "Character/KulkiEnemyPawn.h"

USTT_StopChasingAfterTime::USTT_StopChasingAfterTime(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = false;
}

EStateTreeRunStatus USTT_StopChasingAfterTime::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	if (Actor)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindWeakLambda(this, [this]()
		{
			Actor->bIdleState = true;
			BroadcastDelegate(StopChase);
		});
		const float Delay = FMath::RandRange(DelayTime - RandomDeviation, DelayTime + RandomDeviation); 
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Delay, false);	
	}
	
	return EStateTreeRunStatus::Running;
}
