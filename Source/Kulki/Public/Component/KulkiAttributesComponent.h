// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KulkiAttributesComponent.generated.h"

class USphereComponent;

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_BODY()

	FAttribute() {}
	
	/* No EditAnywhere because Value is setting in BeginPlay. Use "Base" variable from Character. */
	UPROPERTY(BlueprintReadOnly)
	float Value = 5.f;

	/* Max value of attribute that can be possessed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="5.0", ClampMin="5.0"))
	float MaxValue = 100.f;

	/* Curve to set how much value add to Attribute based on eaten Enemy's Attribute value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> AddToValueCurve = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float /*NewValue*/);
DECLARE_DELEGATE(FOnAttributeReachedZero);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KULKI_API UKulkiAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKulkiAttributesComponent();
	
	UFUNCTION(BlueprintCallable, Category="Kulki")
	void SetStrengthAttribute(float NewStrength, UStaticMeshComponent* Mesh, USphereComponent* AttackSphereCollision, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable, Category="Kulki")
	void AddToStrengthAttribute(float EnemyStrength, UStaticMeshComponent* Mesh, USphereComponent* AttackSphereCollision, float& OutMovementSpeed);
		
	UFUNCTION(BlueprintCallable, Category="Kulki")
	void SetSpeedAttribute(float NewSpeed, float& OutMovementSpeed);

	UFUNCTION(BlueprintCallable, Category="Kulki")
	void AddToSpeedAttribute(float EnemySpeed, float& OutMovementSpeed);

	FOnAttributeChanged OnStrengthChangedDelegate;
	FOnAttributeChanged OnSpeedChangedDelegate;

	/* Base value from which character's movement speed is calculated. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.0", ClampMin="0.0"), Category="Kulki|Speed")
	float BaseMovementSpeed = 500.f;

	/* Character's movement speed can't go below this value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin="10.0", ClampMin="10.0"), Category="Kulki|Speed")	
	float MinMovementSpeed = 200.f;
	
	/* Character's movement speed can't surpass this value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin="10.0", ClampMin="10.0"), Category="Kulki|Speed")
	float MaxMovementSpeed = 1500.f;

	/* Used to multiply Speed Attribute value. Increases Movement speed.
	 * Movement speed = BaseMovementSpeed + (SpeedAttribute * SpeedMultiplier) - (StrengthAttribute * SpeedPenaltyMultiplier) 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.1", ClampMin="0.1"), Category="Kulki|Speed")
	float SpeedMultiplier = 10.f;

	/* Used to multiply Strength Attribute value. Decreases Movement speed.
	 * Movement speed = BaseMovementSpeed + (SpeedAttribute * SpeedMultiplier) - (StrengthAttribute * SpeedPenaltyMultiplier) 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.1", ClampMin="0.1"), Category="Kulki|Speed")
	float SpeedPenaltyMultiplier = 5.f;

	/* Used to multiply Strength Attribute value. Increases scale of the mesh.
	 * NewScale = StrengthAttribute * SizeMultiplier
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.01", ClampMin="0.01"), Category="Kulki|Strength")
	float SizeMultiplier = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
    FAttribute StrengthAttribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
    FAttribute SpeedAttribute;

	FOnAttributeReachedZero OnAttributeReachedZero;
	
protected:
	virtual void BeginPlay() override;
	
private:
	void SetOwnerSize(UStaticMeshComponent* Mesh, USphereComponent* AttackSphereCollision);
	void SetOwnerSpeed(float& OutMovementSpeed);
	
};
