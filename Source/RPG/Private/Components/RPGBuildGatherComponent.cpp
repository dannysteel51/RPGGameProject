// Copyright Dan Stull

#include "Components/RPGBuildGatherComponent.h"
#include "FoliageStatiMesh/StoneInstaceStaticMeshComp.h"
#include "AbilitySystem/RPGAbilitySystemBlueprintLibrary.h"
#include "Characters/RPGCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "FoliageStatiMesh/BushInstanceStaticMeshComp.h"
#include "FoliageStatiMesh/TreeInstanceStaticMeshComp.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

struct FUIWidgetRow;

URPGBuildGatherComponent::URPGBuildGatherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ItemType = EItemType::none;
	Character = nullptr;
	/*
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HarvestingNiagaraSystem(TEXT("NiagaraSystem'/Game/Systems/NS_Harvesting.NS_Harvesting'"));
	if(HarvestingNiagaraSystem.Succeeded())
	{
		HarvestingNiagara = HarvestingNiagaraSystem.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HarvestingNiagaraRadialSystem(TEXT("NiagaraSystem'/Game/Systems/NS_HarvestingRadial.NS_HarvestingRadial'"));
	if(HarvestingNiagaraRadialSystem.Succeeded())
	{
		HarvestingNiagaraRadial = HarvestingNiagaraRadialSystem.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HarvestingAnimationSystem(TEXT("AnimMontage'/Game/Animations/Player/Player_Harvesting.Player_Harvesting'"));
	if(HarvestingAnimationSystem.Succeeded())
	{
		HarvestingAnimation = HarvestingAnimationSystem.Object;
	}
	*/
}

void URPGBuildGatherComponent::SetItemType(const EItemType& InItemType)
{
	ItemType = InItemType;
}

void URPGBuildGatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGBuildGatherComponent::BreakStone()
{
	UE_LOG(LogTemp, Warning, TEXT("BreakStone"));
	switch (ItemType)
	{
		case EItemType::Stone:
		{
			if(UStoneInstaceStaticMeshComp* Stone = Cast<UStoneInstaceStaticMeshComp>(StoneInstance))
				Stone->RemoveInstance(StoneHarvestableItemIndex);
			break;
		}
		case EItemType::Tree:
		{
			Cast<UTreeInstanceStaticMeshComp>(TreeInstance)->RemoveInstance(TreeHarvestableItemIndex);
			break;
		}
		case EItemType::Bush:
		{
			Cast<UBushInstanceStaticMeshComp>(BushInstance)->RemoveInstance(BushHarvestableItemIndex);
			break;
		}
		default:
			break;
	}
	
}

void URPGBuildGatherComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;
	Character  = Cast<ARPGCharacter>(Owner);
}

void URPGBuildGatherComponent::LineTraceForHarvestableItem(const EItemType& InItemType ,EItemType& OutItemType, bool& OutHitHarvestableItem)
{
	FHitResult HitResult;
	HitResult = URPGAbilitySystemBlueprintLibrary::LineTraceFromCharacterCamera(Character,750.f);
	if(HitResult.bBlockingHit)
	{
		HarvestableImpactPoint = HitResult.ImpactPoint;

		if(InItemType == EItemType::Tree)
		{
			SetItemType(EItemType::Tree);
			if(Cast<UTreeInstanceStaticMeshComp>(HitResult.GetComponent()))
			{
				TreeInstance = Cast<UPrimitiveComponent>(HitResult.GetComponent());
				HitHarvestableComponent = HitResult.GetComponent();
				TreeHarvestableItemIndex = HitResult.Item;
				HitTreeArray.Add(TreeHarvestableItemIndex);
				CanInteractWithTree = true;
				OutItemType = EItemType::Tree;
				OutHitHarvestableItem = true;
			}
			else
			{
				TreeInstance = nullptr;
				CanInteractWithTree = false;
				CanInteractWithStone = false;
				CanInteractWithBush = false;
				OutItemType = EItemType::none;
				OutHitHarvestableItem = false;
			}
			
		}
		if (InItemType == EItemType::Stone)
		{
			SetItemType(EItemType::Stone);
			if(Cast<UStoneInstaceStaticMeshComp>(HitResult.GetComponent()))
			{
				StoneInstance = Cast<UPrimitiveComponent>(HitResult.GetComponent());
				HitHarvestableComponent = HitResult.GetComponent();
				StoneHarvestableItemIndex = HitResult.Item;
				Cast<UStoneInstaceStaticMeshComp>(StoneInstance)->StoneIndex = StoneHarvestableItemIndex;
				HitStoneArray.Add(StoneHarvestableItemIndex);
				CanInteractWithStone = true;
				OutItemType = EItemType::Stone;
				OutHitHarvestableItem = true;
			}
			else
			{
				StoneInstance = nullptr;
				CanInteractWithTree = false;
				CanInteractWithStone = false;
				CanInteractWithBush = false;
				OutItemType = EItemType::none;
				OutHitHarvestableItem = false;
			}
			
		}
		if (InItemType == EItemType::Bush)
		{
			SetItemType(EItemType::Bush);
			if(Cast<UBushInstanceStaticMeshComp>(HitResult.GetComponent()))
			{
				BushInstance = Cast<UPrimitiveComponent>(HitResult.GetComponent());
				HitHarvestableComponent = HitResult.GetComponent();
				BushHarvestableItemIndex = HitResult.Item;
				HitBushArray.Add(BushHarvestableItemIndex);
				CanInteractWithBush = true;
				OutItemType = EItemType::Bush;
				OutHitHarvestableItem = true;
			}
			else
			{
				BushInstance = nullptr;
				CanInteractWithTree = false;
				CanInteractWithStone = false;
				CanInteractWithBush = false;
				OutItemType = EItemType::none;
				OutHitHarvestableItem = false;
			}
		}
	}
	else
	{
		TreeInstance = nullptr;
		StoneInstance = nullptr;
		BushInstance = nullptr;
		CanInteractWithTree = false;
		CanInteractWithStone = false;
		CanInteractWithBush = false;
		ItemType = EItemType::none;
	}
}

void URPGBuildGatherComponent::InteractingWithItems(const EItemType& InItemType)
{
	switch (InItemType)
	{
	case EItemType::Tree:
		if(CanInteractWithTree)
		{
			//CutDownTree();
		}
		break;
	case EItemType::Stone:
		if(CanInteractWithStone)
		{
			if(UStoneInstaceStaticMeshComp* Stone = Cast<UStoneInstaceStaticMeshComp>(StoneInstance))
			{
				Stone->SetBuildGatherComponent(this);

				for (int32 i = 0; i < HitStoneArray.Num(); i++)
				{
					if (HitStoneArray[i] == Stone->StoneIndex)
					{
						InteractWithStones();

						UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
						if (AnimInstance && HarvestingAnimation)
						{
							AnimInstance->Montage_Play(HarvestingAnimation, 2.f, EMontagePlayReturnType::MontageLength, 0.0f, true);
							CanUseAxe = true;
							GetWorld()->GetTimerManager().SetTimer(HarvestTimerHandle, this, &URPGBuildGatherComponent::HarvestEffects, 0.3f, false);
						}
					}
				}
			}
		}
		break;
	case EItemType::Bush:
		if(CanInteractWithBush)
		{
			//HarvestBush();
		}
	default:
		break;
	}
}

void URPGBuildGatherComponent::InteractWithStones()
{
	if(CanInteractWithStone)
	{
		if(UStoneInstaceStaticMeshComp* Stone = Cast<UStoneInstaceStaticMeshComp>(StoneInstance))
		{
			Stone->InteractingWithRock = true;
			Stone->HasAppliedDamageToRockYet = false;
			PlayerDamageToRock = UKismetMathLibrary::RandomIntegerInRange(7, 21);
			Stone->IncomingDamage = PlayerDamageToRock;
			CanUseAxe = false;
		}
	}
}

void URPGBuildGatherComponent::HarvestEffects() const
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HarvestingNiagara, HarvestableImpactPoint, FRotator::ZeroRotator, FVector(0.5f));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HarvestingNiagaraRadial, HarvestableImpactPoint, FRotator::ZeroRotator, FVector(0.5f));
}


void URPGBuildGatherComponent::MineTheRock_Implementation()
{
	EItemType OutItemType;
	bool OutLocalFound;
	LineTraceForHarvestableItem(EItemType::Stone, OutItemType, OutLocalFound);
	if(OutLocalFound)
	{
		//MineRock();
		InteractingWithItems(EItemType::Stone);
	}
}

void URPGBuildGatherComponent::CutDownTree_Implementation()
{
	EItemType OutItemType;
	bool OutLocalFound;
	LineTraceForHarvestableItem(EItemType::Tree, OutItemType, OutLocalFound);
	if(OutLocalFound)
	{
		//CutTree();
		InteractingWithItems(EItemType::Tree);
	}
}

void URPGBuildGatherComponent::HarvestBush_Implementation()
{
	EItemType OutItemType;
	bool OutLocalFound;
	LineTraceForHarvestableItem(EItemType::Bush, OutItemType, OutLocalFound);
	if(OutLocalFound)
	{
		//HarvestBush();
		InteractingWithItems(EItemType::Bush);
	}
}

