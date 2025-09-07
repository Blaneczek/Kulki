// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "KulkiBasePawn.generated.h"

struct FOnAttributeChangeData;
class UGameplayAbility;
class UGameplayEffect;
class UFloatingPawnMovement;
class USphereComponent;
class UKulkiAttributeSet;

UCLASS()
class KULKI_API AKulkiBasePawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKulkiBasePawn();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UKulkiAttributeSet* GetAttributeSet() const { return AttributeSet; }
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	void InitDefaultAttributes();
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level);

	void AddCharacterAbilities();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<UStaticMeshComponent> KulkiMesh;
	
	/* Capsule used to interact with target's DefendCapsule. Radius scales with mesh size. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> AttackSphereCollision;
	
	/* Capsule used to interact with target's AttackCapsule. Radius stays the same (very small in center of mesh). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> DefendSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UKulkiAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Kulki|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditAnywhere, Category="Kulki|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

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
	
private:
	void SetKulkiPawnSize(const FOnAttributeChangeData& Data);
	void SetKulkiMovementSpeed(const FOnAttributeChangeData& Data);
};
