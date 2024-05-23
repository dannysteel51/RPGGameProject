
#include "Characters/RPGCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/RPGPlayerController.h"
#include "Player/RPGPlayerState.h"
#include "UI/HUD/RPGHUD.h"
#include "MotionWarpingComponent.h"
#include "VectorTypes.h"
#include "Components/CustomMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/DebugHelper.h"
#include "Windows/WindowsApplication.h"


ARPGCharacter::ARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Level Up Niagara Component");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Elementalist;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* ARPGCharacter::FindClosestCollectableActor(AActor* InActor)
{
	InActors.Add(InActor);
	for (AActor* Actor : InActors)
	{
		const ARPGCharacter* RPGCharacter = Cast<ARPGCharacter>(Actor);
		if (Actor == RPGCharacter) continue;
		float Old = UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), GetActorLocation());
		NewDistance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());
		if (NewDistance < CurrentDistance)
		{
			CurrentDistance = NewDistance;
			ClosestActor = Actor;
		}
		else if (NewDistance > CurrentDistance)
		{
			CurrentDistance = CurrentDistance;
			ClosestActor = ClosestActor;
		}
	}
	return ClosestActor;
}


void ARPGCharacter::InitAbilityActorInfo()
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	RPGPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(RPGPlayerState, this);
	
	Cast<URPGAbilitySystemComponent>(RPGPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet(); // sent to ability system component to set up the delegate
	
	AbilitySystemComponent = RPGPlayerState->GetAbilitySystemComponent();
	AttributeSet = RPGPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FRPGGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ARPGCharacter::StunTagChanged);
	
	if (ARPGPlayerController* RPGPlayerController = Cast<ARPGPlayerController>(GetController()))
	{
		if (ARPGHUD* RPGHUD = Cast<ARPGHUD>(RPGPlayerController->GetHUD()))
		{
			RPGHUD->InitOverlay(RPGPlayerController, RPGPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void ARPGCharacter::DodgeFinished()
{
	bDodge = false;
}

void ARPGCharacter::RemoveGameplayEffect()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
	if (ASC == nullptr) return;
	//TSubclassOf<UGameplayEffect> GameplayEffectClass = EquippedWeapon->GetItemAttributeEffect();
	//AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, ASC);
}

void ARPGCharacter::PossessedBy(AController* NewController)
{
	// For Server
	Super::PossessedBy(NewController);
	InitAbilityActorInfo(); //Created function used to set abilitySystemComponent and AttributeSet as well as call/use the ability system component's InitAbilityActorInfo function to set owner actor and avatar actor
	AddCharacterAbilities();
}

void ARPGCharacter::OnRep_PlayerState()
{
	// For Client
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void ARPGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); // Calls the base class implementation
}

int32 ARPGCharacter::GetPlayerLevel_Implementation()
{
	const ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	return RPGPlayerState->GetPlayerLevel();
}
void ARPGCharacter::AddToXP_Implementation(int32 InXP)
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	RPGPlayerState->AddToXP(InXP);
}

int32 ARPGCharacter::GetXP_Implementation() const
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	return RPGPlayerState->GetXP();
}

void ARPGCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void ARPGCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 ARPGCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	return RPGPlayerState->LevelUpInfo->FindLevelForExperience(InXP);
}

int32 ARPGCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	
	return RPGPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 ARPGCharacter::GetAttributePoints_Implementation() const
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	return RPGPlayerState->GetAttributePoints();
}

int32 ARPGCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);
	
	return RPGPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void ARPGCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	RPGPlayerState->AddToAttributePoints(InAttributePoints);
}

void ARPGCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	// TODO: Add spell points to the player state
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	RPGPlayerState->AddToSpellPoints(InSpellPoints);
}

void ARPGCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	RPGPlayerState->AddToLevel(InPlayerLevel);

	if (URPGAbilitySystemComponent* RPGAbilitySystemComponent = Cast<URPGAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		RPGAbilitySystemComponent->UpdateAbilityStatuses(RPGPlayerState->GetPlayerLevel());
	}
}

void ARPGCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (ARPGPlayerController* RPGPlayerController = Cast<ARPGPlayerController>(GetController()))
	{
		RPGPlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void ARPGCharacter::HideMagicCircle_Implementation()
{
	if (ARPGPlayerController* RPGPlayerController = Cast<ARPGPlayerController>(GetController()))
	{
		RPGPlayerController->HideMagicCircle();
	}
}



