// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StateTree/Tasks/STT_RunEQSTick.h"

#include "EnvironmentQuery/EnvQueryManager.h"

USTT_RunEQSTick::USTT_RunEQSTick(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = true;
	Actor = nullptr;
	EnvQuery = nullptr;
	OutLocation = FVector::ZeroVector;
}

EStateTreeRunStatus USTT_RunEQSTick::EnterState(FStateTreeExecutionContext& Context,
                                                const FStateTreeTransitionResult& Transition)
{
	if (EnvQuery && Actor)
	{
		EnvQueryRequest = FEnvQueryRequest(EnvQuery,	Actor);
	}
	else
	{
		return EStateTreeRunStatus::Failed;
	}
		
	return Super::EnterState(Context, Transition);
}

EStateTreeRunStatus USTT_RunEQSTick::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	
	EnvQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &USTT_RunEQSTick::QueryFinished);
	
	return EStateTreeRunStatus::Running;
}

void USTT_RunEQSTick::QueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (const FEnvQueryResult* EnvResult = Result.Get())
	{
		OutLocation = EnvResult->IsSuccessful() ? EnvResult->GetItemAsLocation(0) : FVector::ZeroVector;
	}
}
