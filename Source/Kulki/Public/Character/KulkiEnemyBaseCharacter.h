// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KulkiEnemyBaseCharacter.generated.h"

class UCapsuleComponent;
class UKulkiAttributesComponent;

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	RED		UMETA(DisplayName = "RED"),
	YELLOW	UMETA(DisplayName = "YELLOW"),
	PURPLE	UMETA(DisplayName = "PURPLE")
};

UENUM(BlueprintType, Blueprintable)
enum class EEnemyState: uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	IDLE	UMETA(DisplayName = "IDLE"),
	CHASE	UMETA(DisplayName = "CHASE"),
	ESCAPE	UMETA(DisplayName = "ESCAPE")
};

UCLASS()
class KULKI_API AKulkiEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AKulkiEnemyBaseCharacter();
	
	UFUNCTION(BlueprintCallable, Category="Kulki")
	UKulkiAttributesComponent* GetAttributesComponent() const { return AttributesComponent; }
	
	void SetAttributesValue(float Strength, float Speed);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Kulki")
	void SetState(EEnemyState NewState);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
    EEnemyType Type;
	
	UPROPERTY(BlueprintReadOnly, Category="Kulki")
	bool bCanChase = true;
	
protected:
	virtual void BeginPlay() override;

	void SetMeshColor();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UStaticMeshComponent> KulkiMesh;

	/* Capsule used to interact with Player's DefendCapsule. Radius scales with mesh size. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UCapsuleComponent> AttackCapsuleCollision;

	/* Capsule used to interact with Player's AttackCapsule. Radius stays the same (very small in center of mesh). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UCapsuleComponent> DefendCapsuleCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
    TObjectPtr<UKulkiAttributesComponent> AttributesComponent;

	/* If Enemy is not spawn by manager, we can set Strength Attribute value here. */ 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
    float TempStrengthAttributeValue = 5.f;
	
	/* If Enemy is not spawn by manager, we can set Speed Attribute value here. */ 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
    float TempSpeedAttributeValue = 5.f;

};
