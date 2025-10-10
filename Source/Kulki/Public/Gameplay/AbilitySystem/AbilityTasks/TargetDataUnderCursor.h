// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FVector&, Data);

/**
 * 
 */
UCLASS()
class KULKI_API UTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Kulki|Ability|Tasks", meta=(DisplayName="TargetDataUnderMouse"), meta=(HidePin="OwningAbility"), meta=(DefaultToSelf="OwningAbility"), BlueprintInternalUseOnly)
	static UTargetDataUnderCursor* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;
};
