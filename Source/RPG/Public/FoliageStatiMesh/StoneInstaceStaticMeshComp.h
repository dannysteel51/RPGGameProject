// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "StoneInstaceStaticMeshComp.generated.h"

class URPGBuildGatherComponent;
class ARPGPlayerController;
class ARPGCharacter;
/**
 * 
 */
UCLASS(BlueprintType)
class RPG_API UStoneInstaceStaticMeshComp : public UFoliageInstancedStaticMeshComponent
{
	GENERATED_BODY()
public:

	UStoneInstaceStaticMeshComp();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	ARPGCharacter* Character;

	UPROPERTY()
	ARPGPlayerController* PlayerController;

	bool HasAppliedDamageToRockYet = false;

	bool InteractingWithRock = false;

	int32 RockLife = 0;

	int32 IncomingDamage = 0;

	int32 RemainingHealth = 0;

	int32 StoneIndex = 0;

	FTimerHandle RockMiningTimerHandle;

	FTimerHandle BreakStoneTimerHandle;

	UPROPERTY()
	URPGBuildGatherComponent* BuildGatherComponent;

	void SetBuildGatherComponent(URPGBuildGatherComponent* InBuildGatherComponent);

	TArray<UUserWidget*> RockMiningWidgetArray;

	TMap<int32, UUserWidget*> RockMiningWidgetMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> RockMiningWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* RockMiningSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* RockMiningFinishSound;

	UFUNCTION()
	void MiningRock();

	int32 ApplyDamageToRock(int32 InDamageAmount);

	void RemoveWidgetFromScreen();

	void DelayForBreakingStone();
};