// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KulkiAttributesComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float /*NewValue*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KULKI_API UKulkiAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKulkiAttributesComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	float GetStrengthValue() const {return Strength; }
	UFUNCTION(BlueprintCallable)
	void SetStrengthValue(float NewStrength);
	
	UFUNCTION(BlueprintCallable)
	float GetSpeedValue() const {return Speed; }
	UFUNCTION(BlueprintCallable)
	void SetSpeedValue(float NewSpeed);

	FOnAttributeChangedSignature OnStrengthChangedDelegate;
	FOnAttributeChangedSignature OnSpeedChangedDelegate;
	
private:
	float Strength;
	float Speed;
};
