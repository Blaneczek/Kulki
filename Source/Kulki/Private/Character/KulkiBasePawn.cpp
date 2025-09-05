// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/KulkiBasePawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AKulkiBasePawn::AKulkiBasePawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	KulkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KulkiMesh"));
	SetRootComponent(KulkiMesh);
	KulkiMesh->CastShadow = false;

	AttackSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphereCollision->SetupAttachment(RootComponent);

	DefendSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DefendSphere"));
	DefendSphereCollision->SetupAttachment(RootComponent);
	
	AttributesComponent = CreateDefaultSubobject<UKulkiAttributesComponent>("AttributesComponent");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

void AKulkiBasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}



