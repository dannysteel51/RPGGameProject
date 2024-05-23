// Copyright Dan Stull


#include "Actor/DungeonTravelEntrance.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/RPGCharacter.h"
#include "Components/BoxComponent.h"
#include "Game/RPGGameInstance.h"

// Sets default values
ADungeonTravelEntrance::ADungeonTravelEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

}

// Called when the game starts or when spawned
void ADungeonTravelEntrance::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADungeonTravelEntrance::OnSphereOverlap);

	const AActor* Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UGameplayStatics::PlaySoundAtLocation(this, EntranceSound, Player->GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EntranceEffect, Player->GetActorLocation());
}

// Called every frame
void ADungeonTravelEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonTravelEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AActor* Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (OtherActor == Player)
	{
		const FVector ActorExitLocation = Player->GetActorLocation() + FVector(0.f, SpawnDistanceFromExit, 0.f);
		URPGGameInstance* GameInstance = Cast<URPGGameInstance>(GetGameInstance());
		if(IsValid(GameInstance))
		{
			GameInstance->PlayerEntranceLocation = ActorExitLocation;
			UGameplayStatics::OpenLevel(GetWorld(), "FirstDungeon");
		}
	}
}

