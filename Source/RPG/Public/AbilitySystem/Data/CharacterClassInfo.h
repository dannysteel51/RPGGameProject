// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

struct FScalableFloat;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger,
	Boss
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<class UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};


/**
 * 
 */
UCLASS()
class RPG_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TObjectPtr<UCurveTable> DamageCalculationsCoefficients;
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

	FCharacterClassDefaultInfo SetClassDefaultInfo(ECharacterClass CharacterClass, FCharacterClassDefaultInfo ClassDefaultInfo);
	
};
