// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonTravelExit.generated.h"

class UNiagaraSystem;
class ARPGCharacter;
class URPGGameInstance;
class UBoxComponent;

UCLASS()
class RPG_API ADungeonTravelExit : public AActor
{
	GENERATED_BODY()
	
public:	

	ADungeonTravelExit();
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon")
	USoundBase* ExitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon")
	UNiagaraSystem* ExitEffect;

public:	

	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
