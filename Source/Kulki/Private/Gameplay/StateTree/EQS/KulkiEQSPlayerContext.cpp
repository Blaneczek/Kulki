// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StateTree/EQS/KulkiEQSPlayerContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

void UKulkiEQSPlayerContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (const APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Player);
	}
}
