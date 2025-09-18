// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StateTree/Evaluators/STE_PlayerEvaluator.h"
#include "Character/KulkiPlayerPawn.h"
#include "Kismet/GameplayStatics.h"

void USTE_PlayerEvaluator::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);

	bHasTick = false;

	PlayerPawn = Cast<AKulkiPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn)
	{
		PlayerAttributeSet = PlayerPawn->GetAttributeSet();
	}
}

void USTE_PlayerEvaluator::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);
	
	PlayerPawn = nullptr;
	PlayerAttributeSet = nullptr;
}


