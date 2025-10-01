// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"

#include "STT_GetRandomLocation.generated.h"

class AKulkiEnemyPawn;
/**
 * 
 */
UCLASS()
class KULKI_API USTT_GetRandomLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	USTT_GetRandomLocation(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	FVector OriginLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output)
	FVector OutLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RadiusFromOrigin;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
