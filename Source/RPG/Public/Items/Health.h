// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Health.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AHealth : public AItem
{
	GENERATED_BODY()

private:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, Category = "Health")
	float AddedHealth = 25;

public:
	FORCEINLINE float GetAddedHealth() const { return AddedHealth; }
	FORCEINLINE void SetAddedHealth(float AmountOfHealth) { AddedHealth = AmountOfHealth;  }

	
};
