// Copyright Dan Stull


#include "Actor/DungeonTravelExit.h"

#include "NiagaraFunctionLibrary.h"
#include "Game/RPGGameInstance.h"
#include "Characters/RPGCharacter.h"
#include "Components/BoxComponent.h"


ADungeonTravelExit::ADungeonTravelExit()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
}

void ADungeonTravelExit::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADungeonTravelExit::OnSphereOverlap);

	const AActor* Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UGameplayStatics::PlaySoundAtLocation(this, ExitSound, Player->GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExitEffect, Player->GetActorLocation());
}

void ADungeonTravelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADungeonTravelExit::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AActor* Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (OtherActor == Player)
	{
		URPGGameInstance* GameInstance = Cast<URPGGameInstance>(GetGameInstance());
		if (IsValid(GameInstance))
		{
			GameInstance->AlreadySetActorLocation = true;
			UGameplayStatics::OpenLevel(GetWorld(), "GaeaMap2");
		}
	}
}

