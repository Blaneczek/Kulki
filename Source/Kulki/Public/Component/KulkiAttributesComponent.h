// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KulkiAttributesComponent.generated.h"

class UCapsuleComponent;

USTRUCT(BlueprintType)
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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float /*NewValue*/);
DECLARE_DELEGATE(FOnAttributeReachedZero);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float BaseMovementSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed")	
	float MinMovementSpeed = 200.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed")
	float MaxMovementSpeed = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CapsulePadding = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float SpeedMultiplier = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float SpeedPenaltyMultiplier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Strength")
	float SizeMultiplier = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FAttribute StrengthAttribute;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FAttribute SpeedAttribute;

	FOnAttributeReachedZero OnAttributeReachedZero;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision);
	void SetOwnerSpeed(float& OutMovementSpeed);
	
};
