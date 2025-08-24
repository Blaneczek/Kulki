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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="5.0", ClampMin="5.0"))
	float Value = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="5.0", ClampMin="5.0"))
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
	void SetStrengthAttribute(float NewStrength, UStaticMeshComponent* Mesh, UCapsuleComponent* AttackCapsuleCollision, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable)
	void AddToStrengthAttribute(float EnemyStrength, UStaticMeshComponent* Mesh, UCapsuleComponent* AttackCapsuleCollision, float& OutMovementSpeed);
		
	UFUNCTION(BlueprintCallable)
	void SetSpeedAttribute(float NewSpeed, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable)
	void AddToSpeedAttribute(float EnemySpeed, float& OutMovementSpeed);

	FOnAttributeChangedSignature OnStrengthChangedDelegate;
	FOnAttributeChangedSignature OnSpeedChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.0", ClampMin="0.0"), Category = "Speed")
	float BaseMovementSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin="10.0", ClampMin="10.0"), Category = "Speed")	
	float MinMovementSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin="10.0", ClampMin="10.0"), Category = "Speed")
	float MaxMovementSpeed = 1500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.1", ClampMin="0.1"), Category = "Speed")
	float SpeedMultiplier = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.1", ClampMin="0.1"), Category = "Speed")
	float SpeedPenaltyMultiplier = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.01", ClampMin="0.01"), Category = "Strength")
	float SizeMultiplier = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.0", ClampMin="0.0"))
    float CapsulePadding = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAttribute StrengthAttribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAttribute SpeedAttribute;

	FOnAttributeReachedZero OnAttributeReachedZero;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision);
	void SetOwnerSpeed(float& OutMovementSpeed);
	
};
