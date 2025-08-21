// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KulkiAttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KULKI_API UKulkiAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKulkiAttributesComponent();

protected:
	virtual void BeginPlay() override;

public:	
	
};
