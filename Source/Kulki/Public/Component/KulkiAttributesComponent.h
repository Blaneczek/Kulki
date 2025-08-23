// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KulkiAttributesComponent.generated.h"

class UCapsuleComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float /*NewValue*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KULKI_API UKulkiAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKulkiAttributesComponent();

	UFUNCTION(BlueprintCallable)
	float GetStrengthAttribute() const {return Strength; }
	
	UFUNCTION(BlueprintCallable)
	void SetStrengthAttribute(float NewStrength, UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable)
	void AddToStrengthAttribute(float ValueToAdd, UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision, float& OutMovementSpeed);
	
	UFUNCTION(BlueprintCallable)
	float GetSpeedAttribute() const {return Speed; }
	
	UFUNCTION(BlueprintCallable)
	void SetSpeedAttribute(float NewSpeed, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable)
	void AddToSpeedAttribute(float ValueToAdd, float& OutMovementSpeed);

	FOnAttributeChangedSignature OnStrengthChangedDelegate;
	FOnAttributeChangedSignature OnSpeedChangedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseMovementSpeed = 500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CapsulePadding = 30.f;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision);
	void SetOwnerSpeed(float& OutMovementSpeed);
	
	float Strength;
	float Speed;
};
