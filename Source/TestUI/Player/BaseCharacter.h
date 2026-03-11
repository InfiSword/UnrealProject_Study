// Copyright (c) 2024, Your Name or Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class TESTUI_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	virtual void OnInteract(ABaseCharacter* Interactor);
	
	UFUNCTION(BlueprintCallable, Category = "Character|Control")
	void SetMovementAndLookEnabled(bool bEnabled);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character|Interaction")
	virtual bool CanInteract(ABaseCharacter* Interactor, float MaxDistance = 500.0f) const;

protected:
	virtual void BeginPlay() override;
};

