// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "RPGBuildGatherComponent.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;
class ARPGCharacter;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Tree,
	Stone,
	Bush,
	none
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class RPG_API URPGBuildGatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGBuildGatherComponent();

	EItemType ItemType;

	void SetItemType(const EItemType& InItemType);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CanStillHitStone = false;

	void BreakStone();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	ARPGCharacter* Character;

	bool LocalFound = false;

	bool CanInteractWithTree = false;

	bool CanInteractWithStone = false;

	bool CanInteractWithBush = false;

	bool CanUseAxe = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HarvestingAnimation = nullptr;

	TArray<int32> HitTreeArray;

	TArray<int32> HitStoneArray;

	TArray<int32> HitBushArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* HarvestingNiagara = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* HarvestingNiagaraRadial = nullptr;

	FTimerHandle HarvestTimerHandle;

	FTransform BuildTransform = FTransform::Identity;

	FVector HarvestableImpactPoint = FVector::ZeroVector;

	UPROPERTY()
	UActorComponent* HitHarvestableComponent = nullptr;

	int32 TreeHarvestableItemIndex = 0;

	int32 StoneHarvestableItemIndex = 0;

	int32 BushHarvestableItemIndex = 0;

	int32 PlayerDamageToRock = 0;

	UPROPERTY()
	UStaticMeshComponent* BuildGhost = nullptr;

	int32 BuildIndex = 0;

	UPROPERTY()
	AActor* HitActor = nullptr;

	UPROPERTY()
	UPrimitiveComponent* HitComponent = nullptr;

	UPROPERTY()
	FVector ImpactPoint = FVector::ZeroVector;

	UPROPERTY()
	UPrimitiveComponent* BushInstance = nullptr;

	UPROPERTY()
	UPrimitiveComponent* StoneInstance = nullptr;

	UPROPERTY()
	UPrimitiveComponent* TreeInstance = nullptr;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MineTheRock();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CutDownTree();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HarvestBush();

	void LineTraceForHarvestableItem(const EItemType& InItemType ,EItemType& OutItemType, bool& OutHitHarvestableItem);

	void InteractingWithItems(const EItemType& InItemType);

	void InteractWithStones();

	void HarvestEffects() const;
	
};
