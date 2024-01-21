// Copyright Dan Stull


#include "AbilitySystem/Abilities/RPGBeamSpell.h"

#include "AbilitySystem/RPGAbilitySystemBlueprintLibrary.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


void URPGBeamSpell::StoreBeamHitDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		BeamHitLocation = HitResult.ImpactPoint;
		BeamHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void URPGBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerCharacter = Cast<ARPGCharacter>(CurrentActorInfo->AvatarActor);
		PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	}
}

void URPGBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	FVector BeamSocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation("WeaponSocket");
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,
		BeamSocketLocation,
		BeamTargetLocation,
		10.f,
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true);
	if(HitResult.bBlockingHit)
	{
		BeamHitLocation = HitResult.ImpactPoint;
		BeamHitActor = HitResult.GetActor();
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(BeamHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &URPGBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &URPGBeamSpell::PrimaryTargetDied);
		}
	}
}

void URPGBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(BeamHitActor);
	
	TArray<AActor*> OverlappingActors;
	URPGAbilitySystemBlueprintLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OverlappingActors, ActorsToIgnore, 850.f, BeamHitActor->GetActorLocation());

	// int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	int32 NumAdditionalTargets = 5;
	
	URPGAbilitySystemBlueprintLibrary::GetClosestTargets(NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, BeamHitActor->GetActorLocation());

	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &URPGBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &URPGBeamSpell::AdditionalTargetDied);
			}
		}
	}
}
