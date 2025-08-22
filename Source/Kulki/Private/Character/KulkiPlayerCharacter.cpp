// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Component/KulkiAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/KulkiHUD.h"

AKulkiPlayerCharacter::AKulkiPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->CastShadow = false;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleCollision->SetupAttachment(RootComponent);
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetRelativeRotation(FRotator(-90.f, 0.0f, 0.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm);
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;

	AttributesComponent = CreateDefaultSubobject<UKulkiAttributesComponent>("AttributesComponent");
}

void AKulkiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set default attributes
	AttributesComponent->SetStrengthAttribute(DebugStrength, SphereMesh, CapsuleCollision, MovementSpeed, true);
	AttributesComponent->SetSpeedAttribute(DebugSpeed, MovementSpeed);
	
	// Init Main widget with controller 
	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (AKulkiHUD* KulkiHUD = Cast<AKulkiHUD>(PC->GetHUD()))
		{
			KulkiHUD->InitOverlayWidget(this);
		}
	}

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerCharacter::OnOverlap);
}

void AKulkiPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AKulkiPlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: try something different 
	if (AKulkiEnemyBaseCharacter* Enemy = Cast<AKulkiEnemyBaseCharacter>(OtherActor))
	{
		float Value = -10.f;
		if (AttributesComponent->GetStrengthValue() >= Enemy->GetAttributesComponent()->GetStrengthValue())
		{
			Value *= -1;
			Enemy->Destroy();	
		}

		switch (Enemy->Type)
		{
			case EEnemyType::RED:
				{
					AttributesComponent->AddToStrengthAttribute(Value, SphereMesh, CapsuleCollision, MovementSpeed);
					break;
				}
			case EEnemyType::YELLOW:
				{
					AttributesComponent->AddToSpeedAttribute(Value, MovementSpeed);
					break;
				}
			case EEnemyType::PURPLE:
				{
					//TODO: find a way to set Value as always negative
					AttributesComponent->AddToStrengthAttribute(-10.f, SphereMesh, CapsuleCollision, MovementSpeed);
					AttributesComponent->AddToSpeedAttribute(-10.f, MovementSpeed);
					break;
				}
			default: break;
		}						
	}
}




