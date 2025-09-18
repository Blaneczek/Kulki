// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "GSTT_GlobalManager.generated.h"

class AKulkiPlayerPawn;
class AKulkiEnemyPawn;
/**
 * 
 */
UCLASS()
class KULKI_API UGSTT_GlobalManager : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	UGSTT_GlobalManager(const FObjectInitializer& ObjectInitializer);

	/** The owning Enemy Pawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context)
	TObjectPtr<AKulkiEnemyPawn> Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<AKulkiPlayerPawn> PlayerPawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output)
	FStateTreeDelegateDispatcher CheckIfBigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output)
	FStateTreeDelegateDispatcher BackToIdle;
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

private:
	void BindEnemyDelegates();
	void BindPlayerDelegates();
};
