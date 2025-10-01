// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/AbilitySystem/AbilityTasks/TargetDataUnderCursor.h"

UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCursor* MyObj = NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderCursor::Activate()
{
	if (APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get())
	{
		FHitResult CursorHit;
		// ECC_GameTraceChannel1 - Floor
		PC->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, CursorHit);	
		ValidData.Broadcast(CursorHit.Location);
	}
	else
	{
		ValidData.Broadcast(FVector::ZeroVector);
	}
	
}
