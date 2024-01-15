
#include "Characters/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CustomMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("Burn Debuff Niagara Component");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FRPGGameplayTags::Get().Debuff_Burn;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "StartupWeaponSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

AActor* ABaseCharacter::GetAvatar_Implementation() 
{
	return this;
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

USkeletalMeshComponent* ABaseCharacter::GetWeapon()
{
	return Weapon;
}

UNiagaraSystem* ABaseCharacter::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage ABaseCharacter:: GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for ( FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(MontageTag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ABaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ABaseCharacter::AddToMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass ABaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered ABaseCharacter::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeath ABaseCharacter::GetOnDeathDelegate()
{
	return OnDeath;
}

void ABaseCharacter::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	if (ECharacterType::ECT_Boss == CharacterType)
	{
		bDead = true;
		// TODO: Boss is going to have a death montage unlike regular enemies
		//float AnimTime = PlayAnimMontage(DeathMontage, 1.f, FName("Default"));
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && DeathMontage)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
			AnimInstance->Montage_Play(DeathMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
	}
	else if (ECharacterType::ECT_RegularEnemy == CharacterType || ECharacterType::ECT_MainCharacter == CharacterType)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
        	
		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Weapon->AddImpulse(DeathImpulse * 0.1, NAME_None, true);
        
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
        	
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bDead = true;
	}
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	/*
	MontageTagToSectionMap.Add(GameplayTags.Montage_Attack_Weapon, WeaponSocketName);
	MontageTagToSectionMap.Add(GameplayTags.Montage_Attack_LeftHand, LeftHandSocketName);
	MontageTagToSectionMap.Add(GameplayTags.Montage_Attack_RightHand, RightHandSocketName);

	for (auto Pair : MontageTagToSectionMap)
	{
		if (MontageTag.MatchesTagExact(Pair.Key))
		{
			return GetMesh()->GetSocketLocation(Pair.Value);
		}
	}
	*/
	const FRPGGameplayTags& GameplayTags = FRPGGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon))
	{
		return Weapon->GetSocketLocation(WeaponSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketname);
	}
	return FVector();
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameEffectClass, float level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameEffectClass);
	
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameEffectClass, level, EffectContext);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ABaseCharacter::AddCharacterAbilities()
{
	URPGAbilitySystemComponent* RPGAbilitySystemComponent = CastChecked<URPGAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!HasAuthority()) return;
	RPGAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	RPGAbilitySystemComponent->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}


