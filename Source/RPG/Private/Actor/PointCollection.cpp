// Copyright Dan Stull


#include "Actor/PointCollection.h"

#include "AbilitySystem/RPGAbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{

	PrimaryActorTick.bCanEverTick = false;

	PT_0 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_0"));
	ImmutablePoints.Add(PT_0);
	SetRootComponent(PT_0);

	PT_1 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_1"));
	ImmutablePoints.Add(PT_1);
	PT_1->SetupAttachment(GetRootComponent());

	PT_2 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_2"));
	ImmutablePoints.Add(PT_2);
	PT_2->SetupAttachment(GetRootComponent());

	PT_3 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_3"));
	ImmutablePoints.Add(PT_3);
	PT_3->SetupAttachment(GetRootComponent());

	PT_4 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_4"));
	ImmutablePoints.Add(PT_4);
	PT_4->SetupAttachment(GetRootComponent());
	
	PT_5 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_5"));
	ImmutablePoints.Add(PT_5);
	PT_5->SetupAttachment(GetRootComponent());

	PT_6 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_6"));
	ImmutablePoints.Add(PT_6);
	PT_6->SetupAttachment(GetRootComponent());

	PT_7 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_7"));
	ImmutablePoints.Add(PT_7);
	PT_7->SetupAttachment(GetRootComponent());
	
	PT_8 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_8"));
	ImmutablePoints.Add(PT_8);
	PT_8->SetupAttachment(GetRootComponent());

	PT_9 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_9"));
	ImmutablePoints.Add(PT_9);
	PT_9->SetupAttachment(GetRootComponent());

	PT_10 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_10"));
	ImmutablePoints.Add(PT_10);
	PT_10->SetupAttachment(GetRootComponent());
}

TArray<USceneComponent*> APointCollection::GetGroundsPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverRide)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("NumPoints is greater than the number of points in the collection"));
	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Point : ImmutablePoints)
	{
		if(ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Point!= PT_0)
		{
			FVector ToPoint = Point->GetComponentLocation() - PT_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverRide, FVector::UpVector);
			Point->SetWorldLocation(PT_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		TArray<AActor*> ActorsToIgnore;
		URPGAbilitySystemBlueprintLibrary::GetLivePlayersWithinRadius(this, ActorsToIgnore, TArray<AActor*>{}, 1500.f, GetActorLocation());

		FCollisionQueryParams Params;
		Params.AddIgnoredActors(ActorsToIgnore);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), Params);

		const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Point->SetWorldLocation(AdjustedLocation);
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Point);
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}


