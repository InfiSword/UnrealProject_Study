// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestCharacter.h"
#include "../Inventory/InventoryComponent.h"

// Sets default values
AMyTestCharacter::AMyTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void AMyTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

