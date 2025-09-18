// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_GetChaseLocation.generated.h"

class AKulkiEnemyPawn;
/**
 * 
 */
UCLASS()
class KULKI_API USTT_GetChaseLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	USTT_GetChaseLocation(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context)
	TObjectPtr<AKulkiEnemyPawn> Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<AActor> Player;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Output)
	FVector OutLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvoidanceStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceFromPlayerWithoutAvoidance;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

private:
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> AvoidanceNeighbours;
};
