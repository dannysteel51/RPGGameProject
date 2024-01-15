// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Interfaces/EnemyInterface.h"
#include "RPGEnemy.generated.h"

class ARPGEffectActor;
class UNiagaraSystem;
class ARPGWeapon;
class UDamageTextComponent;
class UWidgetComponent;
class UBehaviorTree;
class ARPGAIController;


/**
 * 
 */
UCLASS()
class RPG_API ARPGEnemy : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ARPGEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	/*
	 * CombatInterface
	 */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	 /* End CombatInterface
	 */

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSigniture OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSigniture OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	bool bHitReacting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Combat)
	float BaseWalkSpeed = 250.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	float LifeSpan = 2.5f;

	UPROPERTY(BlueprintReadWrite, Category = Combat)
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	FHitResult HitResult;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	void SpawnItem() const;
	void SpawnWeapon() const;
	void SpawnEffect() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterClassDefaults")
	int32 EnemyLevel = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ARPGAIController> RPGAIController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARPGWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARPGEffectActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* SpawnEffectParticle = nullptr;
};
