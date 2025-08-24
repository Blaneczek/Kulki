// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KulkiGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KULKI_API UKulkiGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 Level;
};
