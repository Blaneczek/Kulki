// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "STE_EnemyEvaluator.generated.h"

class UKulkiAttributeSet;
class AKulkiEnemyPawn;
/**
 * 
 */
UCLASS()
class KULKI_API USTE_EnemyEvaluator : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta=(AllowPrivateAccess="true"))
	TObjectPtr<AKulkiEnemyPawn> Actor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Output, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UKulkiAttributeSet> EnemyAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Output, meta=(AllowPrivateAccess="true"))
	FVector SpawnLocation;

protected:
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;
	virtual void TreeStop(FStateTreeExecutionContext& Context) override;
};
