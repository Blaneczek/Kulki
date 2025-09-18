// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "STT_RunEQSTick.generated.h"


/**
 * 
 */
UCLASS()
class KULKI_API USTT_RunEQSTick : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	USTT_RunEQSTick(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Context)
	TObjectPtr<AActor> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UEnvQuery> EnvQuery;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Output)
	FVector OutLocation;
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

private:
	UPROPERTY()
	FEnvQueryRequest EnvQueryRequest;
	
	void QueryFinished(TSharedPtr<FEnvQueryResult> Result);
};
