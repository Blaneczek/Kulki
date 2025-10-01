// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "KulkiBasePawn.h"
#include "KulkiDoppelgangerPawn.generated.h"

class AKulkiPlayerPawn;

DECLARE_DELEGATE(FOnCanMerge);

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

	/* Ability owner ref. */
	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	/* Time after Doppelganger can merge with Player's pawn.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Kulki")
	float MergeTime;

	/* Used to add attributes back to Player. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TSubclassOf<UGameplayEffect> MergeEffectClass;
	
private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
