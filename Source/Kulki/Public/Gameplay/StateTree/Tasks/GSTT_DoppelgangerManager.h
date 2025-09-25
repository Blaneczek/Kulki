// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "GSTT_DoppelgangerManager.generated.h"

class AKulkiDoppelgangerPawn;
/**
 * 
 */
UCLASS()
class KULKI_API UGSTT_DoppelgangerManager : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
public:
	UGSTT_DoppelgangerManager(const FObjectInitializer& ObjectInitializer);

	/** The owning Enemy Pawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context)
	TObjectPtr<AKulkiDoppelgangerPawn> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output)
	FStateTreeDelegateDispatcher Merge;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

};
