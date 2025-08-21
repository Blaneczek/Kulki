// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KulkiEnemyBaseCharacter.generated.h"


UCLASS()
class KULKI_API AKulkiEnemyBaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	AKulkiEnemyBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	

};
