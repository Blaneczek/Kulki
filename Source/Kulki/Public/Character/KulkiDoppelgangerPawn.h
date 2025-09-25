// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KulkiBasePawn.h"
#include "KulkiDoppelgangerPawn.generated.h"

class UProjectileMovementComponent;
class AKulkiPlayerPawn;

DECLARE_MULTICAST_DELEGATE(FOnCanMerge);

UCLASS()
class KULKI_API AKulkiDoppelgangerPawn : public AKulkiBasePawn
{
	GENERATED_BODY()

public:
	AKulkiDoppelgangerPawn();

	void SetData(AActor* AbilityOwner, float InMergeTime);

	void MergeWithOwner();
	
	bool bCanMerge = false;
	FOnCanMerge OnCanMerge;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	float MergeTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TSubclassOf<UGameplayEffect> MergeEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
