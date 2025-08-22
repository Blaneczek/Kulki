// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Component/KulkiAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/KulkiHUD.h"

AKulkiPlayerCharacter::AKulkiPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereMesh->CastShadow = false;
	
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

	// Set initial values and size
	AttributesComponent->SetStrengthValue(3.f);
	AttributesComponent->SetSpeedValue(1.f);
	AttributesComponent->SetOwnerSize(SphereMesh, GetCapsuleComponent());
	
	// Init Main widget with controller 
	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (AKulkiHUD* KulkiHUD = Cast<AKulkiHUD>(PC->GetHUD()))
		{
			KulkiHUD->InitOverlayWidget(this);
		}
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerCharacter::OnOverlap);
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
		if (AttributesComponent->GetStrengthValue() >= Enemy->GetAttributesComponent()->GetStrengthValue())
		{
			switch (Enemy->Type)
			{
			case EEnemyType::RED:
				{
					AttributesComponent->SetStrengthValue(AttributesComponent->GetStrengthValue() + 0.5f);
					break;
				}
			case EEnemyType::YELLOW:
				{
					AttributesComponent->SetSpeedValue(AttributesComponent->GetSpeedValue() + 0.5f);
					break;
				}
			case EEnemyType::PURPLE:
				{
					AttributesComponent->SetStrengthValue(AttributesComponent->GetStrengthValue() - 0.5f);
					AttributesComponent->SetSpeedValue(AttributesComponent->GetSpeedValue() - 0.5f);
					break;
				}
			default: break;
			}
			
			AttributesComponent->SetOwnerSize(SphereMesh, GetCapsuleComponent());
			Enemy->Destroy();
		}
		else
		{
			switch (Enemy->Type)
			{
			case EEnemyType::RED:
				{
					AttributesComponent->SetStrengthValue(AttributesComponent->GetStrengthValue() - 0.5f);
					break;
				}
			case EEnemyType::YELLOW:
				{
					AttributesComponent->SetSpeedValue(AttributesComponent->GetSpeedValue() - 0.5f);
					break;
				}
			case EEnemyType::PURPLE:
				{
					AttributesComponent->SetStrengthValue(AttributesComponent->GetStrengthValue() - 0.5f);
					AttributesComponent->SetSpeedValue(AttributesComponent->GetSpeedValue() - 0.5f);
					break;
				}
			default: break;
			}
			
			AttributesComponent->SetOwnerSize(SphereMesh, GetCapsuleComponent());
		}		
	}
}




