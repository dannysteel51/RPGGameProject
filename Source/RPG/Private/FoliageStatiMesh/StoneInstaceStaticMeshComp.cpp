// Copyright Dan Stull


#include "FoliageStatiMesh/StoneInstaceStaticMeshComp.h"
#include "Blueprint/UserWidget.h"
#include "Characters/RPGCharacter.h"
#include "Components/RPGBuildGatherComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RPGPlayerController.h"
#include "UI/DamageToCollectable.h"

UStoneInstaceStaticMeshComp::UStoneInstaceStaticMeshComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	Character = nullptr;
	PlayerController = nullptr;
	HasAppliedDamageToRockYet = false;
	InteractingWithRock = false;
	RockLife = 0;
	IncomingDamage = 0;
	RemainingHealth = 0;
	StoneIndex = 0;
	RockMiningTimerHandle.Invalidate();
	BreakStoneTimerHandle.Invalidate();
	BuildGatherComponent = nullptr;
	RockMiningWidgetClass = nullptr;
	RockMiningSound = nullptr;
	RockMiningFinishSound = nullptr;
}

void UStoneInstaceStaticMeshComp::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;
	Character  = Cast<ARPGCharacter>(Owner);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PC)) return;
	PlayerController = Cast<ARPGPlayerController>(PC);

	RockLife = UKismetMathLibrary::RandomFloatInRange(25, 75);
}

void UStoneInstaceStaticMeshComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(InteractingWithRock && !HasAppliedDamageToRockYet)
	{
		HasAppliedDamageToRockYet = true;
		MiningRock();
	}
}

void UStoneInstaceStaticMeshComp::SetBuildGatherComponent(URPGBuildGatherComponent* InBuildGatherComponent)
{
	BuildGatherComponent = InBuildGatherComponent;
}

void UStoneInstaceStaticMeshComp::MiningRock()
{
	ApplyDamageToRock(IncomingDamage);

	if(RockMiningWidgetClass)
	{
		UDamageToCollectable* CollectDamageWidget = CreateWidget<UDamageToCollectable>(PlayerController, RockMiningWidgetClass);
		if(CollectDamageWidget)
		{
			CollectDamageWidget->IncomingDamage = IncomingDamage;

			RockMiningWidgetArray.Add(CollectDamageWidget);

			for (int32 i = 0; i < RockMiningWidgetArray.Num(); i++)
			{
				if (RockMiningWidgetArray.Last() == RockMiningWidgetArray[i])
				{
					//RockMiningWidgetMap.Add(0, RockMiningWidgetArray.[i]->Animation);
					FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
					RockMiningWidgetArray[i]->SetPositionInViewport(ViewportSize / 2, true);
					RockMiningWidgetArray[i]->AddToViewport();
					UGameplayStatics::PlaySound2D(GetWorld(), RockMiningSound);

					GetWorld()->GetTimerManager().SetTimer(RockMiningTimerHandle, this, &UStoneInstaceStaticMeshComp::RemoveWidgetFromScreen, 1.85f, false);

					if(RockLife <= 0)
					{
						if(IsValid(BuildGatherComponent))
						{
							BuildGatherComponent->CanStillHitStone = false;
							UGameplayStatics::PlaySound2D(GetWorld(), RockMiningFinishSound);
							RockLife = UKismetMathLibrary::RandomFloatInRange(25, 75);
							GetWorld()->GetTimerManager().SetTimer(BreakStoneTimerHandle, this, &UStoneInstaceStaticMeshComp::DelayForBreakingStone, 0.45f, false);
						}
					}
				}
			}
		}
	}
	
}

int32 UStoneInstaceStaticMeshComp::ApplyDamageToRock(int32 InDamageAmount)
{
	RockLife -= InDamageAmount;
	return RockLife;
}

void UStoneInstaceStaticMeshComp::RemoveWidgetFromScreen()
{
	for (auto const RockMiningWidget : RockMiningWidgetArray)
	{
		if(IsValid(RockMiningWidget))
		{
			RockMiningWidget->RemoveFromParent();
		}
		RockMiningTimerHandle.Invalidate();
	}
}

void UStoneInstaceStaticMeshComp::DelayForBreakingStone()
{
	BuildGatherComponent->BreakStone();
	BreakStoneTimerHandle.Invalidate();
}
