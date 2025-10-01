// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/KulkiAIController.h"
#include "Components/StateTreeAIComponent.h"

AKulkiAIController::AKulkiAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>("StateTreeAIComponent");
}

void AKulkiAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKulkiAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindWeakLambda(this, [this]()
	{
		if (StateTreeAIComponent)
        {
        	StateTreeAIComponent->StartLogic();
        }
	});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
}


