// Copyright Dan Stull


#include "Characters/RPGEnemy.h"

#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/RPGAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "Actor/RPGEffectActor.h"
#include "Actor/BaseItem/RPGWeapon.h"
#include "AI/RPGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Public/RPGGameplayTags.h"
#include "UI/Widget/RPGUserWidget.h"

ARPGEnemy::ARPGEnemy()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	BaseWalkSpeed = 250.f;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); // Minimal for AI character

	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	HealthBar->SetupAttachment(GetRootComponent());

	InitialLifeSpan = 0.f;
	
}

void ARPGEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	RPGAIController = Cast<ARPGAIController>(NewController);
	RPGAIController->RunBehaviorTree(BehaviorTree);
	RPGAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	
}

int32 ARPGEnemy::GetPlayerLevel_Implementation()
{
	return EnemyLevel;
}

void ARPGEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	SpawnItem(); 
	SpawnEffect();
	if (RPGAIController) RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die(DeathImpulse);
}

void ARPGEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ARPGEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ARPGEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if(HasAuthority())
	{
		URPGAbilitySystemBlueprintLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	if (URPGUserWidget* RPGUserWidget = Cast<URPGUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		RPGUserWidget->SetWidgetController(this);
	}
	
	if (const URPGAttributeSet* RPGAttributeSet = Cast<URPGAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FRPGGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &ARPGEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(RPGAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(RPGAttributeSet->GetMaxHealth());
	}
	
}

void ARPGEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (RPGAIController&& RPGAIController->GetBlackboardComponent())
	{
		RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void ARPGEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (RPGAIController && RPGAIController->GetBlackboardComponent())
	{
		RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}


void ARPGEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FRPGGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ARPGEnemy::StunTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void ARPGEnemy::InitializeDefaultAttributes() const
{
	URPGAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(this, CharacterClass, EnemyLevel, AbilitySystemComponent);
}

void ARPGEnemy::SpawnWeapon() const
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
		ARPGWeapon* SpawnedWeapon = World->SpawnActor<ARPGWeapon>(WeaponClass, SpawnLocation, FRotator::ZeroRotator);
		if (SpawnedWeapon)
		{
			// TODO:: Set the weapon's epic type
		}
	}
}

void ARPGEnemy::SpawnEffect() const
{
	UWorld* World = GetWorld();
	if (World && SpawnEffectParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, SpawnEffectParticle, GetActorLocation() + FVector(0.f, 0.f, 50.f));
	}
}
