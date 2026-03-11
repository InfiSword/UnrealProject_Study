// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopNPC.h"
#include "../Datas/DataManager.h"
#include "../Player/MyTestCharacter.h"
#include "Shop.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"

AShopNPC::AShopNPC()
{
	PrimaryActorTick.bCanEverTick = false;
	SyncSoldOutStateSize();
	CurrentInteractor = nullptr;
	bIsShopOpen = false;
	ActiveShopWidget = nullptr;
}

void AShopNPC::BeginPlay()
{
	Super::BeginPlay();
	BuildItemsForSaleFromDataManager();
	NormalizeItemPrices();
	SyncSoldOutStateSize();
}

void AShopNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShopNPC::OnInteract(ABaseCharacter* Interactor)
{
	Super::OnInteract(Interactor);

	if (!Interactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OnInteract - Interactor is nullptr"));
		return;
	}

	AMyTestCharacter* PlayerInteractor = Cast<AMyTestCharacter>(Interactor);
	if (!PlayerInteractor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OnInteract - Interactor is not MyTestCharacter"));
		return;
	}

	// Toggle shop open/close
	if (bIsShopOpen)
	{
		UE_LOG(LogTemp, Log, TEXT("ShopNPC::OnInteract - Closing shop (already open)"));
		IShopInterface::Execute_CloseShop(this);
		return;
	}

	// Use interface to open shop
	IShopInterface::Execute_OpenShop(this, PlayerInteractor);
}

bool AShopNPC::OpenShop_Implementation(AMyTestCharacter* Player)
{
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OpenShop_Implementation - Player is nullptr"));
		return false;
	}

	if (bIsShopOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OpenShop_Implementation - Shop is already open"));
		return false;
	}

	if (!CanInteract(Player, InteractionDistance))
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OpenShop_Implementation - Player is too far away"));
		return false;
	}

	if (!ShopWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OpenShop_Implementation - ShopWidgetClass is not set"));
		return false;
	}

	APlayerController* PC = Cast<APlayerController>(Player->GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OpenShop_Implementation - PlayerController is null"));
		return false;
	}

	// Close existing widget if any
	if (ActiveShopWidget)
	{
		ActiveShopWidget->RemoveFromParent();
		ActiveShopWidget = nullptr;
	}

	// Create shop widget
	UShop* NewShopWidget = CreateWidget<UShop>(PC, ShopWidgetClass);
	if (!NewShopWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopNPC::OpenShop_Implementation - Failed to create shop widget"));
		return false;
	}

	ActiveShopWidget = NewShopWidget;
	CurrentInteractor = Player;
	bIsShopOpen = true;

	// Add to viewport and initialize
	NewShopWidget->AddToViewport();
	NewShopWidget->OpenShop(this);
	Player->SetMovementAndLookEnabled(false);

	UE_LOG(LogTemp, Log, TEXT("ShopNPC::OpenShop_Implementation - Shop opened for %s"), *Player->GetName());
	return true;
}

void AShopNPC::CloseShop_Implementation()
{
	if (!bIsShopOpen)
	{
		UE_LOG(LogTemp, Verbose, TEXT("ShopNPC::CloseShop_Implementation - Shop is already closed"));
		return;
	}

	AMyTestCharacter* PlayerInteractor = Cast<AMyTestCharacter>(CurrentInteractor);
	
	// Remove widget
	if (ActiveShopWidget)
	{
		ActiveShopWidget->RemoveFromParent();
		ActiveShopWidget = nullptr;
	}

	bIsShopOpen = false;
	CurrentInteractor = nullptr;

	// Restore player movement
	if (PlayerInteractor)
	{
		PlayerInteractor->SetMovementAndLookEnabled(true);
	}

	UE_LOG(LogTemp, Log, TEXT("ShopNPC::CloseShop_Implementation - Shop closed"));
}

bool AShopNPC::BuildItemsForSaleFromDataManager()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return false;

	UDataManager* DataManager = GameInstance->GetSubsystem<UDataManager>();
	if (!DataManager) return false;

	const TArray<FShopItem> RandomItems = DataManager->BuildRandomShopItems(ShopSlotCount);
	if (RandomItems.Num() <= 0) return false;

	ItemsForSale = RandomItems;

	return true;
}

void AShopNPC::NormalizeItemPrices()
{
	for (FShopItem& Item : ItemsForSale)
	{
		if (!Item.ItemData) continue;

		if (Item.BuyPrice <= 0)
			Item.BuyPrice = FMath::Max(1, Item.ItemData->DefaultBuyPrice);

		if (Item.SellPrice <= 0)
			Item.SellPrice = Item.ItemData->DefaultSellPrice > 0 
				? Item.ItemData->DefaultSellPrice 
				: FMath::Max(1, Item.BuyPrice / 2);
	}
}

void AShopNPC::SyncSoldOutStateSize()
{
	if (SoldOutStates.Num() != ItemsForSale.Num())
	{
		SoldOutStates.SetNumZeroed(ItemsForSale.Num());
	}
}

void AShopNPC::MarkItemSoldOut(int32 Index)
{
	SyncSoldOutStateSize();
	if (SoldOutStates.IsValidIndex(Index))
	{
		SoldOutStates[Index] = true;
	}
}

bool AShopNPC::IsItemSoldOut(int32 Index) const
{
	return SoldOutStates.IsValidIndex(Index) ? SoldOutStates[Index] : false;
}

void AShopNPC::ResetSoldOutStates()
{
	SyncSoldOutStateSize();
	for (bool& State : SoldOutStates)
		State = false;
}

void AShopNPC::RebuildShopDataForTest()
{
	if (!BuildItemsForSaleFromDataManager() && ItemsForSale.Num() > 1)
	{
		const FShopItem FirstItem = ItemsForSale[0];
		ItemsForSale.RemoveAt(0);
		ItemsForSale.Add(FirstItem);
	}

	NormalizeItemPrices();
	ResetSoldOutStates();
}
