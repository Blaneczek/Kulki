// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_StopChasingAfterTime.generated.h"

class AKulkiEnemyPawn;
/**
 * 
 */
UCLASS()
class KULKI_API USTT_StopChasingAfterTime : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	USTT_StopChasingAfterTime(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Context)
	TObjectPtr<AKulkiEnemyPawn> Actor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomDeviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Output)
	FStateTreeDelegateDispatcher StopChase;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
