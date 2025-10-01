// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/StateTree/Tasks/STT_GetRandomLocation.h"
#include "NavigationSystem.h"

USTT_GetRandomLocation::USTT_GetRandomLocation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = false;
	RadiusFromOrigin = 0.f;
	OriginLocation = FVector::ZeroVector;
	OutLocation = FVector::ZeroVector;
}

EStateTreeRunStatus USTT_GetRandomLocation::EnterState(FStateTreeExecutionContext& Context,
                                                       const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	
	//DrawDebugSphere(GetWorld(), OriginLocation, RadiusFromOrigin, 15, FColor::White, false, 30.f);
	if (const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation ResultLocation;
        NavSys->GetRandomReachablePointInRadius(OriginLocation, RadiusFromOrigin, ResultLocation);
		OutLocation = FVector(ResultLocation.Location.X, ResultLocation.Location.Y, 42.f);
	}
	else
	{
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}
