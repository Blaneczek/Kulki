// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KulkiAIController.generated.h"

class UStateTreeAIComponent;

UCLASS()
class KULKI_API AKulkiAIController : public AAIController
{
	GENERATED_BODY()

public:
	AKulkiAIController();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
};
