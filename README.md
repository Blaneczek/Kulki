# Getting Started
<sub>Unreal Engine version 5.6</sub>

Clone the repository or download the ZIP, generate Visual Studio files, and compile using the IDE.

# KULKI
Kulki is a simple game in which the player's task is to eat weaker and avoid stronger enemies.

![kulki](https://github.com/user-attachments/assets/f8d3b9e0-f522-4871-8319-0f2b060e674f)

# Introduction
There are 2 main attributes that affect all aspects of the game. Despite the simplicity of the game I wanted to use the Gameplay Ability System. The **Strength** attribute is responsible for the size of the ball, and the **Speed** attribute for movement speed.

# Key elements

|                                                                               | Description                                                     |
|-------------------------------------------------------------------------------|-----------------------------------------------------------------|
| [Movement](#movement-code)                                                    | Mouse movement.                                                 |
| [Gameplay Ability System](#gameplay-ability-system-code)                      | Use of GAS in the project.                                      |
| [Enemies](#enemies-code-spawncode-ai)                           				| Spawn system and AI behaviour.                                  |

# Movement ([code](Source/Kulki/Private/Player/KulkiPlayerController.cpp))
<details>
<summary>More</summary>
<br>
Movement is controlled with the mouse. The player's ball follows the cursor when you click the right button. 

```c++
void AKulkiPlayerController::FollowMouseCursor()
{
	if (!PlayerPawn)
	{
		return;
	}

	FHitResult HitResult;
	// ECC_GameTraceChannel1 - Floor
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);	
	if (HitResult.bBlockingHit)
	{		
		const FVector HitDirection = (HitResult.ImpactPoint - PlayerPawn->GetActorLocation()).GetSafeNormal();
		PlayerPawn->AddMovementInput(FVector(HitDirection.X, HitDirection.Y, 0.f));
	}
}
```

All movement properties can be set in the PlayerPawn blueprint.
<img src="https://github.com/user-attachments/assets/4ba97b53-6649-4abd-9b22-6da080f0d8e7" width="800">

```c++
  /* Base value from which character's movement speed is calculated. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="0.0", ClampMin="0.0"), Category="Kulki|Speed")
	float BaseMovementSpeed = 500.f;

	/* Character's movement speed can't go below this value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="10.0", ClampMin="10.0"), Category="Kulki|Speed")	
	float MinMovementSpeed = 200.f;
	
	/* Character's movement speed can't surpass this value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin="10.0", ClampMin="10.0"), Category="Kulki|Speed")
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
```
</details>

# Gameplay Ability System ([code](Source/Kulki/Private/Gameplay/AbilitySystem))
<details>
<summary>More</summary>
	
## Attribute Set
Player and enemies have the same Attribute Set, in which two main Attributes are declared. 

```c++
UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UKulkiAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS(UKulkiAttributeSet, MaxStrength);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UKulkiAttributeSet, Speed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UKulkiAttributeSet, MaxSpeed);
```

<br>
As the Strength value changes, the size of the pawn changes, and as the Speed changes, the movement speed changes. Therefore, in the base pawn class, we bind the functions to GameplayAttributeValueChangeDelegate.

```c++
void AKulkiBasePawn::InitAbilityActorInfo()
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AKulkiBasePawn::InitAbilityActorInfo || AbilitySystemComponent is not valid"));
	checkf(IsValid(AttributeSet), TEXT("AKulkiBasePawn::InitAbilityActorInfo || AttributeSet is not valid"));
	
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetStrengthAttribute()).AddUObject(this, &AKulkiBasePawn::SetKulkiPawnSize);
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetSpeedAttribute()).AddUObject(this, &AKulkiBasePawn::SetKulkiMovementSpeed);	
}
```


## Gameplay Effects
Gameplay effects are used to set attributes. Initially, as default values and during interactions with enemies. 

```c++
void AKulkiPlayerPawn::InitDefaultAttributes()
{
	if (DefaultAttributes)
	{
		ApplyEffectToSelf(DefaultAttributes, 1.f);
	}	
}
```
```c++
void AKulkiBasePawn::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) 
{
	if (!GetAbilitySystemComponent() || !GameplayEffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data.Get());
}
```

<img src="https://github.com/user-attachments/assets/01c783c2-03f0-4477-9ab7-01fe394310b4" width="800">

<br>When interacting with enemies, attributes can be added or subtracted. To use only one GameplayEffect for both operations, I created a custom MMC class that returns a positive or negative value that will affect the player. 

<img src="https://github.com/user-attachments/assets/ad4d9a71-2a8d-459b-a7ff-101272ce0674" width="800">

<br>After checking both Player and Enemy Strength, the coefficient is determined. 

```c++
void AKulkiPlayerPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	IKulkiCombatInterface* CombatInterface = Cast<IKulkiCombatInterface>(OtherActor);
	if (bIsImmune || !CombatInterface)
	{
		return;
	}
	
	const float EnemyStrength = CombatInterface->GetStrength();
	float Coefficient = -1.f;

	// If Player is bigger, the coefficient is positive to increase Player's attributes
    if (AttributeSet->GetStrength() >= EnemyStrength)
    {
        Coefficient = 1.f;
    }
	
	bool bEatableEnemy = false;
	CombatInterface->ApplyOverlapEffect(GetAbilitySystemComponent(), Coefficient, bEatableEnemy);
	if (Coefficient > 0.f)
	{
		if (bEatableEnemy) OnEatableEnemyKilled.ExecuteIfBound();
	}
	else
	{
		ActivateImmunity();
	}
}
```

<br>The coefficient is used in the custom MMC thanks to setting the Magnitude by the code and Gameplay Tags (SetSetByCallerMagnitude/GetSetByCallerMagnitude).

```c++
void AKulkiEnemyPawn::ApplyOverlapEffect(UAbilitySystemComponent* TargetASC, float Coefficient, bool& OutIsEatableEnemy)
{
	if (!TargetASC || !GetAbilitySystemComponent() || !OverlapGameplayEffectClass)
	{
		return;
	}

	OutIsEatableEnemy = Type == EEnemyType::RED || Type == EEnemyType::YELLOW;

	// If enemy is Purple and was smaller than player
	bool bWasSmallerPurple = !OutIsEatableEnemy && Coefficient > 0.f;
	
	// If enemy is Purple decrease player's stats regardless size
	Coefficient = OutIsEatableEnemy ? Coefficient : -1.f;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(OverlapGameplayEffectClass, 1.f, ContextHandle);
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::GameplayEffect_Coefficient.GetTag(), Coefficient);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), TargetASC);

	// Enemy was smaller than the player
	if (bWasSmallerPurple || Coefficient > 0.f)
	{				
		SetLifeSpan(0.01f);
	}
}
```

```c++
float UMMC_Attribute::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const float Coefficient = Spec.GetSetByCallerMagnitude(KulkiGameplayTags::GameplayEffect_Coefficient.GetTag(), false, 1.f);

	FAggregatorEvaluateParameters Params;
	float AttributeValue = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, Params, AttributeValue);

	const float CurveValue = AttributeCurve ? AttributeCurve->GetFloatValue(AttributeValue) : 0.f;
	
	return Coefficient * CurveValue;
}
```

## Gameplay Ability
In the game, the player has the Gameplay Ability that creates and throws their copy to catch faster opponents. That action decreases Strength by half. After some time player can merge with Doppelganger and take over its Strength.

![doppelganger](https://github.com/user-attachments/assets/1845ee12-f2e2-4417-9d90-073aa2f6b16d)

```c++
void UKulkiDoppelganger::SpawnDoppelganger(const FVector& TargetLocation)
{
	if (!DoppelgangerPawnClass || !DoppelgangerEffectClass)
	{
		return;
	}
	
	const FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Direction = (TargetLocation - OwnerLocation).GetSafeNormal2D();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(OwnerLocation);
	SpawnTransform.SetRotation(Direction.ToOrientationQuat());
	AKulkiDoppelgangerPawn* Doppelganger = GetWorld()->SpawnActorDeferred<AKulkiDoppelgangerPawn>(
		DoppelgangerPawnClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
                    				
	if (IsValid(Doppelganger))
	{
		Doppelganger->SetData(GetAvatarActorFromActorInfo(), MergeTime);
		UGameplayStatics::FinishSpawningActor(Doppelganger, SpawnTransform);
		Doppelganger->InitAbilityActorInfo();
		Doppelganger->GetKulkiMesh()->AddImpulse(Direction * ImpulseForce);
		
		UAbilitySystemComponent* SourceASC = GetActorInfo().AbilitySystemComponent.Get();
        const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DoppelgangerEffectClass, 1.f, SourceASC->MakeEffectContext());    
        SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Doppelganger->GetAbilitySystemComponent());
	}	
}
```

<img src="https://github.com/user-attachments/assets/82cd31fa-b190-4351-87f3-e08cf68f7eaf" width="800">

</details>

# Enemies ([code spawn](Source/Kulki/Private/Component/KulkiEnemyComponent.cpp))([code AI](Source/Kulki/Private/Gameplay/StateTree))
<details>
<summary>More</summary>
	
<br>**Yellow ball:** adds Speed if we eat it, subtracts it if it eats us
<br>**Red ball:** adds Strength if we eat it, subtracts it if it eats us
<br>**Purple ball:** subtracts Strength and Speed if we eat it or it eats us	

## Enemy Component
Enemies are spawned at the beginning of the game. The further away they are from the player, the higher their attribute values. These values can be set using a float curve where the X axis is the distance from the player and the Y axis is the enemy's Strength/Speed. 

<img src="https://github.com/user-attachments/assets/68e998d7-bc25-4a8e-a3fd-e947d63d9f8c" width="800">

<br>Information about enemy spawns can be set in DataAsset: for each color, we select distance ranges (e.g., 5 yellow balls will spawn between 1000 and 1500 from the player).

<img src="https://github.com/user-attachments/assets/f58c2307-3a02-495b-a8d7-ecef96c072a4" width="800">

<br> Calculating a random location in a given range. Distance from the player and the spawn location will be used to set the attribute values. 
```c++
FVector UKulkiEnemyComponent::CalculateValidRandomLocation(const FVector& PlayerLocation, const FSpawnDistanceRange& DistanceRange, float& OutDistanceFromPlayer, bool& OutFoundValidLocation)
{
	const FVector RandomDirection = UKismetMathLibrary::RandomUnitVector().GetSafeNormal2D();	
	OutDistanceFromPlayer = FMath::RandRange(DistanceRange.MinDistance, DistanceRange.MaxDistance);
	
	const FVector RandomLocationFromPlayer = PlayerLocation + FVector(RandomDirection.X * OutDistanceFromPlayer, RandomDirection.Y * OutDistanceFromPlayer ,42.f);

	if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation SpawnNavLocation;
		NavSystem->ProjectPointToNavigation(RandomLocationFromPlayer, SpawnNavLocation) ? OutFoundValidLocation = true : OutFoundValidLocation = false;
	}

	return FVector(RandomLocationFromPlayer.X, RandomLocationFromPlayer.Y, 42.f);
}
```

```c++
void UKulkiEnemyComponent::SpawnEnemy(const FVector& SpawnLocation, const TPair<EEnemyType, FSpawnEnemyData>& EnemyData, float DistanceFromPlayer, float DifficultyLevelScale)
{
	checkf(EnemyClass, TEXT("UKulkiEnemyComponent::SpawnEnemy | EnemyClass is not valid"));
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	AKulkiEnemyPawn* Enemy = GetWorld()->SpawnActorDeferred<AKulkiEnemyPawn>(
		EnemyClass,
		SpawnTransform,
		GetOwner(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
                    				
	if (IsValid(Enemy) && EnemyData.Value.StrengthToDistanceCurve && EnemyData.Value.SpeedToDistanceCurve)
	{
		Enemy->Type = EnemyData.Key;
		Enemy->SetOverlapGameplayEffectClass(*OverlapGameplayEffectClasses.Find(EnemyData.Key));
		const float Strength = EnemyData.Value.StrengthToDistanceCurve->GetFloatValue(DistanceFromPlayer) * DifficultyLevelScale;
		const float Speed = EnemyData.Value.SpeedToDistanceCurve->GetFloatValue(DistanceFromPlayer) * DifficultyLevelScale;	
		UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);	
		Enemy->InitSpawn(Strength, Speed);

		Enemies.Add(Enemy);		
		if (EnemyData.Key != EEnemyType::PURPLE)
		{
			NumberOfEatableEnemies++;
		}
	}				
}
```

<br>The Strength and Speed values of opponents scale with the selected difficulty level. The multiplier is set in the same DataAsset, and the difficulty level is set after clicking Start Game in the menu.

<img src="https://github.com/user-attachments/assets/ced9ea94-7b2b-4d66-9a10-fa363cd3932d" width="800">

## State Tree AI
Enemies have 3 states:
<br>**IDLE:** They move to a random location in range from the origin location (spawn location)
<br>**CHASE:** If they're bigger, they chase the player.
<br>**ESCAPE:** If they're smaller, they move away from the player.
  
<img src="https://github.com/user-attachments/assets/dc58131c-a192-447c-81a4-3b87d8b2706c" width="800">

<br>In ESCAPE state, when chasing the player, enemies use avoidance to prevent clumping.
 
```c++
FVector USTT_GetChaseLocation::GetAvoidanceLocation()
{
	if (!Actor || !Player)
	{
		FinishTask(false);
		return FVector::ZeroVector;
	}
	
	AvoidanceNeighbors = Actor->AvoidanceNeighbors;
	const FVector ActorLocation = Actor->GetActorLocation();
	const FVector PlayerLocation = Player->GetActorLocation();

	const float ActorDistanceFromPlayer = FVector::DistSquared(ActorLocation, PlayerLocation);	
	if (AvoidanceNeighbors.IsEmpty() || ActorDistanceFromPlayer < FMath::Square(DistanceFromPlayerWithoutAvoidance))
	{
		// Early return, enemy is close enough to player or there are no neighbors
		return PlayerLocation;
	}
	
	FVector Avoidance = FVector::ZeroVector;
	for (const auto& OtherEnemy : AvoidanceNeighbors)
	{
		if (const AActor* OtherActor = OtherEnemy.Get())
		{
			Avoidance += (((ActorLocation - OtherActor->GetActorLocation()).GetSafeNormal()) * AvoidanceStrength);
		}
	}

	return PlayerLocation + Avoidance;
}
```

<br>Without avoidance

![without](https://github.com/user-attachments/assets/e0c551bc-d9f1-4e71-8a6e-d398fdab589f)

<br>With avoidance

![with](https://github.com/user-attachments/assets/a4e05034-8e55-4ac8-add2-72a76ea4a65b)

</details>
