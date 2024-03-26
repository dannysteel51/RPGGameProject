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
	
	SceneSpawnPoints = CreateDefaultSubobject<USceneComponent>(FName("SpawnPoint"));
	SceneSpawnPoints->SetupAttachment(RootComponent);

}

void AEnemyAreaSpawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARPGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerController = Cast<ARPGPlayerController>(GetWorld()->GetFirstPlayerController());
	
	SpawnedEnemyLocations.Add(SceneSpawnPoints->GetComponentLocation());
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
				if (bShouldSpawnEnemy && World && SpawnedEnemyClass != nullptr && !bShouldAutoSpawn)
				{
					AActor* EnemyInstigator = World->SpawnActor(SpawnedEnemyClass, &SpawnedEnemyLocations[i]);
					if (APawn* EnemyPawn = Cast<APawn>(EnemyInstigator))
					{
						EnemyPawn->SpawnDefaultController();
					}
                    				
					EnemyInstigator->OnDestroyed.AddDynamic(this, &AEnemyAreaSpawn::EnemyDestroyed);
				}
			}
		}
	}
}

void AEnemyAreaSpawn::EnemyDestroyed(AActor* DestroyedActor)
{
	EnemyCount--;
}

void AEnemyAreaSpawn::ShowAfterBattleMessage()
{
	bCreateWidget = true;
}

