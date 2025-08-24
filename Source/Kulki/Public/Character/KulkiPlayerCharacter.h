// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/KulkiAttributesComponent.h"
#include "GameFramework/Character.h"
#include "KulkiPlayerCharacter.generated.h"

class UKulkiAttributesComponent;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DELEGATE(FOnImmunityActivation);
DECLARE_DELEGATE(FOnImmunityDeactivation);
DECLARE_DELEGATE(FOnEnemyKilled);

UCLASS()
class KULKI_API AKulkiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AKulkiPlayerCharacter();
	
	UFUNCTION(BlueprintCallable, Category="Kulki")
	UKulkiAttributesComponent* GetAttributesComponent() const { return AttributesComponent; }

	UFUNCTION(BlueprintCallable, Category="Kulki")
	float GetMovementSpeed() const { return MovementSpeed; }
	
	UFUNCTION(BlueprintCallable, Category="Kulki")
	float GetMovementForce() const { return MovementForce; }

	FOnImmunityActivation OnImmunityActivation;
	FOnImmunityDeactivation OnImmunityDeactivation;
	FOnEnemyKilled OnEnemyKilled;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<UStaticMeshComponent> KulkiMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
    TObjectPtr<UKulkiAttributesComponent> AttributesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UCapsuleComponent> AttackCapsuleCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UCapsuleComponent> DefendCapsuleCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin="5.0", ClampMin="5.0"), Category="Kulki|Attributes")
	float BaseStrengthAttributeValue = 30.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin="5.0", ClampMin="5.0"), Category="Kulki|Attributes")
	float BaseSpeedAttributeValue = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin="0.0", ClampMin="0.0"), Category="Kulki|Movement") //TODO: UIMin/Clamp - minimum to move
	float MovementForce = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(UIMin="0.1", ClampMin="0.1"), Category="Kulki|Immunity")
	float ImmunityTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Kulki|Immunity")
	FLinearColor ImmunityColor;
	
private:
	UFUNCTION()
	void OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapDefend(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ActivateImmunity();
	
	void DeactivateImmunity(FLinearColor Color);

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
	
	float MovementSpeed;
	bool bIsImmune = false;

};
