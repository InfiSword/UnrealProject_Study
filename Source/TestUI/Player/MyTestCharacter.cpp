#include "MyTestCharacter.h"
#include "../Inventory/InventoryComponent.h"
#include "BaseCharacter.h"
#include "EngineUtils.h"
#include "../Shop/IShopInterface.h"

AMyTestCharacter::AMyTestCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AMyTestCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyTestCharacter::OnInteractKeyPressed()
{
	// Find nearby actors that implement IShopInterface
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* TargetActor = *ActorItr;

		if (TargetActor == this)
			continue;
		
		if (TargetActor->Implements<UShopInterface>())
		{
			if (ABaseCharacter* BaseChar = Cast<ABaseCharacter>(TargetActor))
			{
				if (BaseChar->CanInteract(this, 500.0f))
				{
					if (IShopInterface::Execute_IsShopOpen(TargetActor))
					{
						IShopInterface::Execute_CloseShop(TargetActor);
						UE_LOG(LogTemp, Log, TEXT("MyTestCharacter: Closed shop via interface"));
					}
					else
					{
						// Open shop via interface
						IShopInterface::Execute_OpenShop(TargetActor, this);
						UE_LOG(LogTemp, Log, TEXT("MyTestCharacter: Opened shop via interface"));
					}
					return;
				}
			}
		}
	}

	// Fallback to general BaseCharacter interaction
	for (TActorIterator<ABaseCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABaseCharacter* TargetCharacter = *ActorItr;
		
		// Skip self
		if (TargetCharacter == this)
			continue;

		// Check if the target can be interacted with
		if (TargetCharacter->CanInteract(this, 500.0f))
		{
			TargetCharacter->OnInteract(this);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No interactable character found nearby"));
}

