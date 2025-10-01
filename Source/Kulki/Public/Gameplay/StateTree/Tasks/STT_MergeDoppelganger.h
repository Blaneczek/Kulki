// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_MergeDoppelganger.generated.h"

class AKulkiAIController;
class AKulkiDoppelgangerPawn;

/**
 * 
 */
UCLASS()
class KULKI_API USTT_MergeDoppelganger : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
public:
	USTT_MergeDoppelganger(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Context)
	TObjectPtr<AKulkiDoppelgangerPawn> Actor;
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
