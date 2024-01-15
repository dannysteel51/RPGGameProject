// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGDamageGameplayAbility.h"
#include "RPGBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGBeamSpell : public URPGDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StoreBeamHitDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

protected:

	UPROPERTY(BlueprintReadWrite, Category = Beam)
	FVector BeamHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = Beam)
	TObjectPtr<AActor> BeamHitActor;

	UPROPERTY(BlueprintReadWrite, Category = Beam)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadWrite, Category = Beam)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = Beam)
	int32 MaxNumShockTargets = 5;
};
