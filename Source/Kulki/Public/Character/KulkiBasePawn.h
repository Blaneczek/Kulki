// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/KulkiAttributesComponent.h"
#include "GameFramework/Pawn.h"
#include "KulkiBasePawn.generated.h"

class UFloatingPawnMovement;
class USphereComponent;

UCLASS()
class KULKI_API AKulkiBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKulkiBasePawn();

	UFUNCTION(BlueprintCallable, Category="Kulki")
	UKulkiAttributesComponent* GetAttributesComponent() const { return AttributesComponent; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<UStaticMeshComponent> KulkiMesh;
	
	/* Capsule used to interact with target's DefendCapsule. Radius scales with mesh size. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> AttackSphereCollision;
	
	/* Capsule used to interact with target's AttackCapsule. Radius stays the same (very small in center of mesh). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> DefendSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
    TObjectPtr<UKulkiAttributesComponent> AttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

};
