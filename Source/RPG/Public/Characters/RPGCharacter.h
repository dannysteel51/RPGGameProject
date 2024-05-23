// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BaseCharacter.h"
#include "Engine.h"
#include "CoreMinimal.h"
#include "Interfaces/PickupInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Items/Weapons/Weapon.h"

#include "RPGCharacter.generated.h"

class ARPGItem;
class ARPGWeapon;
class UCameraComponent;
class USpringArmComponent;
class UCustomMovementComponent;
class UMotionWarpingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegateSigniture, int32, CurrentSlotIndex, int32, NewSlotIndex);

UCLASS()
class RPG_API ARPGCharacter : public ABaseCharacter, public IPickupInterface, public IPlayerInterface
{
	GENERATED_BODY()

public:
	ARPGCharacter();
	virtual void Tick(float DeltaTime) override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInWater = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteractWithBox = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInteractingWithBox = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSpeedBurstActive = false;

	/*
	 * CombatInterface
	 */
	virtual int32 GetPlayerLevel_Implementation() override;
	/*
	 * End CombatInterface
	 */

	/*
	 * PlayerInterface
	 */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() const override;
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircle_Implementation() override;
	/*
	 * End PlayerInterface
	 */
	
	UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }
	
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	
	UAnimMontage* GetDodgeMontage() const { return DodgeMontage; }

	bool GetDodge() const { return bDodge; }

	void SetDodge(bool Dodge) { bDodge = Dodge; }

	bool GetInShockLoop() const { return bInShockLoop; }
	
	bool GetInBlockLoop() const { return bInBlockLoop; }

	bool GetInBowShotLoop() const { return bInBowShotLoop; }
	
protected:
	virtual void BeginPlay() override;

	/*
	 * Collectable Pickup
	 */
	UPROPERTY()
	TArray<AActor*> InActors;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	float NewDistance = 0.0f;

	UPROPERTY()
	float CurrentDistance = 10000.0f;

	UPROPERTY()
	AActor* ClosestActor = nullptr;
	
	UFUNCTION(BlueprintCallable)
	AActor* FindClosestCollectableActor(AActor* InActor);

	/*
	 * End Collectable Pickup
	 */
	
	UPROPERTY()
	AItem* OverlappingItem = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	bool bDodge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bInShockLoop = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bInBlockLoop = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bInBowShotLoop = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionWarpingComponent* MotionWarpingComponent;
	
private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(BlueprintCallable)
	void DodgeFinished();
	
	void RemoveGameplayEffect();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
