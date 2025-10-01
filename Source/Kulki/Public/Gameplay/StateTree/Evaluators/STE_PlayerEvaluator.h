// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "STE_PlayerEvaluator.generated.h"

class UKulkiAttributeSet;
class AKulkiPlayerPawn;
/**
 * 
 */
UCLASS(Blueprintable)
class KULKI_API USTE_PlayerEvaluator : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;
	virtual void TreeStop(FStateTreeExecutionContext& Context) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output, meta=(AllowPrivateAccess="true"))
	TObjectPtr<AKulkiPlayerPawn> PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UKulkiAttributeSet> PlayerAttributeSet;

};
