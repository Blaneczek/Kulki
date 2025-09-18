// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_CheckIfBigger.generated.h"

class AKulkiEnemyPawn;
class UKulkiAttributeSet;
/**
 * 
 */
UCLASS()
class KULKI_API USTT_CheckIfBigger : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
public:
	USTT_CheckIfBigger(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Context)
	TObjectPtr<AKulkiEnemyPawn> Actor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	TObjectPtr<UKulkiAttributeSet> PlayerAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	TObjectPtr<UKulkiAttributeSet> EnemyAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output)
	FStateTreeDelegateDispatcher StartChase;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output)
	FStateTreeDelegateDispatcher StartEscape;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
