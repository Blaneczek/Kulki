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

UCLASS()
class KULKI_API AKulkiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AKulkiPlayerCharacter();
	
	UFUNCTION(BlueprintCallable)
	UKulkiAttributesComponent* GetAttributesComponent() const { return AttributesComponent; }

	UFUNCTION(BlueprintCallable)
	float GetMovementSpeed() const { return MovementSpeed; }

	float GetMovementForce() const { return MovementForce; }

	FOnImmunityActivation OnImmunityActivation;
	FOnImmunityDeactivation OnImmunityDeactivation;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SphereMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UKulkiAttributesComponent> AttributesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> CapsuleCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kulki | Attributes")
	float DefaultStrengthAttribute = 30.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kulki | Attributes")
	float DefaultSpeedAttribute = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kulki")
	float MovementForce = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kulki")
	float ImmunityTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kulki")
	FLinearColor ImmunityColor;
	
private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ActivateImmunity();
	
	UFUNCTION()
	void DeactivateImmunity(FLinearColor Color);

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
	
	float MovementSpeed;
	bool bIsImmune = false;

};
