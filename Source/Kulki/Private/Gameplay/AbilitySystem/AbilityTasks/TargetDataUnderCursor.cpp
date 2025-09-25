// Fill out your copyright notice in the Description page of Project Settings.


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
