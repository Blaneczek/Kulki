// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KulkiAttributesComponent.generated.h"

USTRUCT(Blueprintable)
struct FAttribute
{
	GENERATED_BODY()

	FAttribute() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> AddToValueCurve = nullptr;
};

class UCapsuleComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float /*NewValue*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KULKI_API UKulkiAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKulkiAttributesComponent();
	
	UFUNCTION(BlueprintCallable)
	void SetStrengthAttribute(float NewStrength, UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable)
	void AddToStrengthAttribute(float EnemyStrength, UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision, float& OutMovementSpeed);
		
	UFUNCTION(BlueprintCallable)
	void SetSpeedAttribute(float NewSpeed, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable)
	void AddToSpeedAttribute(float EnemySpeed, float& OutMovementSpeed);

	FOnAttributeChangedSignature OnStrengthChangedDelegate;
	FOnAttributeChangedSignature OnSpeedChangedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseMovementSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)	
	float MinMovementSpeed = 200.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxMovementSpeed = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CapsulePadding = 30.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FAttribute StrengthAttribute;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FAttribute SpeedAttribute;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision);
	void SetOwnerSpeed(float& OutMovementSpeed);

};
