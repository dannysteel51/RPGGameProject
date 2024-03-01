// Copyright Dan Stull


#include "Actor/EnemyAreaSpawn.h"
#include "Characters/RPGCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Characters/RPGEnemy.h"
#include "RPG/DebugHelper.h"
#include "Player/RPGPlayerController.h"

AEnemyAreaSpawn::AEnemyAreaSpawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	for (int i = 0; i < InitialSpawnPoints; i++)
	{
		SpawnPoints = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint" + i));
		SpawnPoints->SetupAttachment(BoxComponent);
		SpawnPointArray.Add(SpawnPoints);
	}
}

void AEnemyAreaSpawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARPGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerController = Cast<ARPGPlayerController>(GetWorld()->GetFirstPlayerController());

	for (int i = 0; i < SpawnPointArray.Num(); i++)
	{
		SpawnedEnemyLocations.Add(SpawnPointArray[i]->GetComponentLocation());
	}
	FVector Position = SpawnedEnemyLocations[0];
	//printf(Position.ToString().c_str());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAreaSpawn::OnBoxComponentOverlapBegin);
}

void AEnemyAreaSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAlreadyOverlapped)
	{
		if (EnemyCount == 0)
		{
			if (!bAlreadyShowedMessage)
			{
				bAlreadyShowedMessage = true;
				ShowAfterBattleMessage();
			}
		}
	}
}

void AEnemyAreaSpawn::OnBoxComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	Debug::Print(FString(TEXT("Overlapped")));
	if (!bAlreadyOverlapped)
	{
		if (OtherActor == PlayerCharacter)
		{
			bAlreadyOverlapped = true;
			for(int i = 0; i < SpawnedEnemyLocations.Num(); i++)
			{
				EnemyCount++;
				// Spawn enemy
				UWorld* World = GetWorld();
				AActor* EnemyInstigator = World->SpawnActor(SpawnedEnemyClass, &SpawnedEnemyLocations[i]);
				EnemyInstigator->OnDestroyed.AddDynamic(this, &AEnemyAreaSpawn::OnEnemyDestroyed);
			}
		}
	}
}

void AEnemyAreaSpawn::OnEnemyDestroyed(AActor* DestroyedActor)
{
	//DestroyedActor->OnDestroyed.RemoveDynamic(this, &AEnemyAreaSpawn::OnEnemyDestroyed);
	EnemyCount--;
}

void AEnemyAreaSpawn::ShowAfterBattleMessage()
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(PlayerController);
	bCreateWidget = true;
	/*
	if (AfterBattleMessageClass)
	{
		AfterBattleMessage = CreateWidget<UUserWidget>(GetWorld(), AfterBattleMessageClass, );
		AfterBattleMessage->AddToViewport();
	}
	*/
}

