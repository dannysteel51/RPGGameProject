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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemySpawn")
	bool bCreateWidget = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySpawn")
	int32 EnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	TSubclassOf<ARPGEnemy> SpawnedEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	USceneComponent* SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	TArray<USceneComponent*> SpawnPointArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	int32 InitialSpawnPoints = 1;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn", meta = (AllowPrivateAccess = "true"))
	TArray <FVector> SpawnedEnemyLocations;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	
	void ShowAfterBattleMessage();
};
