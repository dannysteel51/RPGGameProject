// Copyright Dan Stull


#include "Actor/OverlapActivateActor.h"
#include "Characters/RPGCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AOverlapActivateActor::AOverlapActivateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("OverlapSphere");
	SetRootComponent(SphereComponent);
}

// Called when the game starts or when spawned
void AOverlapActivateActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AOverlapActivateActor::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AOverlapActivateActor::OnSphereOverlapEnd);
}

void AOverlapActivateActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	RPGCharacter = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (OtherActor == RPGCharacter)
	{
		bIsActivated = true;
	}
}

void AOverlapActivateActor::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

// Called every frame
void AOverlapActivateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

