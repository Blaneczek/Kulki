// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/EQS/KulkiEQSPlayerContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

void UKulkiEQSPlayerContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, UGameplayStatics::GetPlayerPawn(this, 0));
}
