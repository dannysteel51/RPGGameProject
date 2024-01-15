// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_OneHanded				UMETA(DisplayName = "One Handed"),
	EWT_TwoHanded				UMETA(DisplayName = "Two Handed"),
	ECS_DefaultMax				UMETA(DisplayName = "Max")
};

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class RPG_API AWeapon : public AItem
{
	GENERATED_BODY()
public:

	AWeapon();

	virtual void BeginPlay() override;

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void DeactivateEmbers();

	void DisableSphereCollision();

	void PlayEquipSound();

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType = EWeaponType::ECS_DefaultMax;

protected:

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

private:

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

};
