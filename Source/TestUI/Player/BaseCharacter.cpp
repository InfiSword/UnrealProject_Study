// Copyright (c) 2024, Your Name or Company. All rights reserved.

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::OnInteract(ABaseCharacter* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("BaseCharacter::OnInteract called by %s"), 
		Interactor ? *Interactor->GetName() : TEXT("nullptr"));
}

void ABaseCharacter::SetMovementAndLookEnabled(bool bEnabled)
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!MovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacter::SetMovementAndLookEnabled - No movement component found"));
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		// NPC or non-player controlled character
		if (bEnabled)
		{
			MovementComp->SetMovementMode(MOVE_Walking);
		}
		else
		{
			MovementComp->DisableMovement();
		}
		return;
	}

	// Player controlled character - handle input mode and cursor
	if (bEnabled)
	{
		MovementComp->SetMovementMode(MOVE_Walking);
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
	}
	else
	{
		MovementComp->DisableMovement();
		PC->SetInputMode(FInputModeGameAndUI());
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}

	UE_LOG(LogTemp, Log, TEXT("BaseCharacter::SetMovementAndLookEnabled - %s movement for %s"), 
		bEnabled ? TEXT("Enabled") : TEXT("Disabled"), *GetName());
}

bool ABaseCharacter::CanInteract(ABaseCharacter* Interactor, float MaxDistance) const
{
	if (!Interactor) return false;
	
	const float Distance = FVector::Dist(GetActorLocation(), Interactor->GetActorLocation());
	return Distance <= MaxDistance;
}

