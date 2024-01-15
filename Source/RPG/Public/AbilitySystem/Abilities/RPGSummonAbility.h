// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "RPGSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGSummonAbility : public URPGGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category = "Summon Settings")
	TSubclassOf<APawn> GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category = "Summon Settings")
	int32 NumberOfMinionsToSpawn = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summon Settings")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summon Settings")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon Settings")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon Settings")
	float SpawnSpread = 90.f;
};
