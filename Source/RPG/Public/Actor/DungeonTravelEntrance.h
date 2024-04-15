// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonTravelEntrance.generated.h"

class UNiagaraSystem;
class ARPGCharacter;
class UBoxComponent;
class URPGGameInstance;

UCLASS()
class RPG_API ADungeonTravelEntrance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonTravelEntrance();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon")
	float SpawnDistanceFromExit = -750.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon")
	USoundBase* EntranceSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon")
	UNiagaraSystem* EntranceEffect;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon")
	UBoxComponent* BoxComponent;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


};
