// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAreaSpawn.generated.h"

class ARPGEnemy;
class UBoxComponent;
class ARPGPlayerController;
class ARPGCharacter;

UCLASS()
class RPG_API AEnemyAreaSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyAreaSpawn();
	bool bAlreadyOverlapped = false;
	bool bAlreadyShowedMessage = false;

	UPROPERTY(BlueprintReadOnly, Category = "EnemySpawn")
	bool bCreateWidget = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySpawn")
	int32 EnemyCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemySpawn")
	TSubclassOf<ARPGEnemy> SpawnedEnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemySpawn")
	TObjectPtr<USceneComponent> SceneSpawnPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	TArray<USceneComponent*> SpawnPointArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySpawn", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> SpawnedEnemyLocations;

	TArray<FVector> EnemySpawnLocations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnemySpawn")
	int32 InitialSpawnPoints = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	UBoxComponent* BoxComponent;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* AfterBattleMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> AfterBattleMessageClass;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ARPGCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ARPGPlayerController* PlayerController;

	

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnBoxComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EnemyDestroyed(AActor* DestroyedActor);
	
	void ShowAfterBattleMessage();
};
