// Fill out your copyright notice in the Description page of Project Settings.
#include "Shop.h"
#include "ShopSlot.h"
#include "ShopNPC.h"
#include "IShopInterface.h"
#include "../Player/MyTestCharacter.h"
#include "../Inventory/InventoryComponent.h"
#include "../Inventory/InventorySlot.h"
#include "../Datas/PartsData.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"

void UShop::NativeConstruct()
{
    Super::NativeConstruct();

    if (MainBuyButton)
    {
        MainBuyButton->OnClicked.RemoveDynamic(this, &UShop::OnMainBuyButtonClicked);
        MainBuyButton->OnClicked.AddDynamic(this, &UShop::OnMainBuyButtonClicked);
    }

    if (MainSellButton)
    {
        MainSellButton->OnClicked.RemoveDynamic(this, &UShop::OnMainSellButtonClicked);
        MainSellButton->OnClicked.AddDynamic(this, &UShop::OnMainSellButtonClicked);
    }

    if (ShopRefreshButton)
    {
        ShopRefreshButton->OnClicked.RemoveDynamic(this, &UShop::OnShopRefreshClicked);
        ShopRefreshButton->OnClicked.AddDynamic(this, &UShop::OnShopRefreshClicked);
    }

    if (MainExitButton)
    {
        MainExitButton->OnClicked.RemoveDynamic(this, &UShop::OnMainExitButtonClicked);
        MainExitButton->OnClicked.AddDynamic(this, &UShop::OnMainExitButtonClicked);
    }

    UpdateSellButtonState();
}

void UShop::NativeDestruct()
{
    UnbindInventoryUpdatedDelegate();

    Super::NativeDestruct();
}

void UShop::UnbindInventoryUpdatedDelegate()
{
    UInventoryComponent* InventoryComp = nullptr;

    if (APawn* OwningPawn = GetOwningPlayerPawn())
    {
        if (AMyTestCharacter* PlayerChar = Cast<AMyTestCharacter>(OwningPawn))
        {
            InventoryComp = PlayerChar->InventoryComponent;
        }
    }

    // 위젯 제거 직전에도 안전하게 이벤트를 해제한다.
    if (IsValid(InventoryComp))
    {
        InventoryComp->OnInventoryUpdated.RemoveDynamic(this, &UShop::HandleInventoryUpdated);
    }
}

void UShop::ClearCurrentSelection()
{
    if (CachedShopSlots.IsValidIndex(CurrentSelectedIndex) && CachedShopSlots[CurrentSelectedIndex])
    {
        CachedShopSlots[CurrentSelectedIndex]->SetSelected(false);
    }

    if (CachedInventorySlots.IsValidIndex(CurrentSelectedInventoryIndex) && CachedInventorySlots[CurrentSelectedInventoryIndex])
    {
        CachedInventorySlots[CurrentSelectedInventoryIndex]->SetSelected(false);
    }

    CurrentSelectedIndex = -1;
    CurrentSelectedPrice = 0;
    CurrentSelectedInventoryIndex = -1;

    UpdateBuyButtonState();
    UpdateSellButtonState();
}

void UShop::BuildShopSlotsOnce()
{
	if (bShopGridInitialized) return;

	if (!ShopSlotGrid)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopMainWidget: 'ShopSlotGrid' BindWidget 연결이 없습니다."));
		return;
	}

	if (!ShopSlotClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopMainWidget: 'ShopSlotClass'가 비어 있습니다. WBP_ShopMain 클래스 기본값에서 설정하세요."));
		return;
	}

	CachedShopSlots.Reset();
	ShopSlotGrid->ClearChildren();

	constexpr int32 MaxShopSlots = 12;
	for (int32 i = 0; i < MaxShopSlots; ++i)
	{
		UShopSlot* NewSlot = CreateWidget<UShopSlot>(this, ShopSlotClass);
		if (!NewSlot) continue;

		CachedShopSlots.Add(NewSlot);
		ShopSlotGrid->AddChildToWrapBox(NewSlot);
	}

	bShopGridInitialized = true;
}

void UShop::OpenShop(AShopNPC* InShop)
{
	if (!InShop)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopMainWidget: OpenShop에 전달된 ShopActor가 null입니다."));
		return;
	}

	CurrentShop = InShop;

	AMyTestCharacter* PlayerChar = Cast<AMyTestCharacter>(GetOwningPlayerPawn());
	if (PlayerChar && PlayerChar->InventoryComponent)
	{
		PlayerChar->InventoryComponent->OnInventoryUpdated.RemoveDynamic(this, &UShop::HandleInventoryUpdated);
		PlayerChar->InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UShop::HandleInventoryUpdated);
	}

	CurrentSelectedIndex = -1;
	CurrentSelectedPrice = 0;
	CurrentSelectedInventoryIndex = -1;
	UpdateBuyButtonState();
	UpdateSellButtonState();

	BuildShopSlotsOnce();
	RebindShopData();
	RefreshInventory();
}

void UShop::RebindShopData()
{
	if (!CurrentShop)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopMainWidget: CurrentShop이 null이라 상점 데이터를 바인딩할 수 없습니다."));
		return;
	}

	const TArray<FShopItem>& Items = CurrentShop->GetItemsForSale();

	for (int32 SlotIndex = 0; SlotIndex < CachedShopSlots.Num(); ++SlotIndex)
	{
		UShopSlot* SlotWidget = CachedShopSlots[SlotIndex];
		if (!SlotWidget) continue;

		if (Items.IsValidIndex(SlotIndex) && Items[SlotIndex].ItemData)
		{
			SlotWidget->SetVisibility(ESlateVisibility::Visible);
			SlotWidget->SetupSlot(Items[SlotIndex], SlotIndex, this);

			if (CurrentShop->IsItemSoldOut(SlotIndex))
			{
				SlotWidget->SetSoldOut();
			}
		}
		else
		{
			SlotWidget->SetEmpty();
		}
	}
}

void UShop::HandleInventoryUpdated()
{
    AMyTestCharacter* PlayerChar = Cast<AMyTestCharacter>(GetOwningPlayerPawn());
    if (!PlayerChar || !PlayerChar->InventoryComponent ||
        !PlayerChar->InventoryComponent->InventoryItems.IsValidIndex(CurrentSelectedInventoryIndex))
    {
        CurrentSelectedInventoryIndex = -1;
    }

    UpdateSellButtonState();
    RefreshInventory();
}

void UShop::RefreshInventory()
{
	AMyTestCharacter* PlayerChar = Cast<AMyTestCharacter>(GetOwningPlayerPawn());
	if (!PlayerChar || !PlayerChar->InventoryComponent || !MyInventoryGrid || !InvenSlotClass) return;

	MyInventoryGrid->ClearChildren();
	CachedInventorySlots.Reset();

	const TArray<FPartInstance>& PartItems = PlayerChar->InventoryComponent->InventoryItems;
	for (int32 i = 0; i < PartItems.Num(); ++i)
	{
		const FPartInstance& PartItem = PartItems[i];
		if (!PartItem.SourceData) continue;

		UInventorySlot* InvenSlot = CreateWidget<UInventorySlot>(this, InvenSlotClass);
		if (InvenSlot)
		{
			InvenSlot->SetupInvenSlot(PartItem, i, false, this);
			
			// 현재 선택된 슬롯이면 강조 표시
			if (i == CurrentSelectedInventoryIndex)
			{
				InvenSlot->SetSelected(true);
			}
			
			CachedInventorySlots.Add(InvenSlot);
			MyInventoryGrid->AddChildToWrapBox(InvenSlot);
		}
	}
}

void UShop::SetSelectedItem(int32 SelectedIndex, int32 ItemPrice)
{
	// 이전 선택된 상점 슬롯 해제
	if (CachedShopSlots.IsValidIndex(CurrentSelectedIndex) && CachedShopSlots[CurrentSelectedIndex])
	{
		CachedShopSlots[CurrentSelectedIndex]->SetSelected(false);
	}

	// 새로운 선택 설정
	CurrentSelectedIndex = SelectedIndex;
	CurrentSelectedPrice = ItemPrice;

	// 새로운 선택 강조
	if (CachedShopSlots.IsValidIndex(CurrentSelectedIndex) && CachedShopSlots[CurrentSelectedIndex])
	{
		CachedShopSlots[CurrentSelectedIndex]->SetSelected(true);
	}

	// 인벤토리 선택 해제
	if (CachedInventorySlots.IsValidIndex(CurrentSelectedInventoryIndex) && CachedInventorySlots[CurrentSelectedInventoryIndex])
	{
		CachedInventorySlots[CurrentSelectedInventoryIndex]->SetSelected(false);
	}
	CurrentSelectedInventoryIndex = -1;

	UpdateBuyButtonState();
	UpdateSellButtonState();
}

void UShop::SetSelectedInventoryItem(int32 InventoryIndex)
{
	// 이전 선택된 인벤토리 슬롯 해제
	if (CachedInventorySlots.IsValidIndex(CurrentSelectedInventoryIndex) && CachedInventorySlots[CurrentSelectedInventoryIndex])
	{
		CachedInventorySlots[CurrentSelectedInventoryIndex]->SetSelected(false);
	}

	// 새로운 선택 설정
	CurrentSelectedInventoryIndex = InventoryIndex;

	// 새로운 선택 강조
	if (CachedInventorySlots.IsValidIndex(CurrentSelectedInventoryIndex) && CachedInventorySlots[CurrentSelectedInventoryIndex])
	{
		CachedInventorySlots[CurrentSelectedInventoryIndex]->SetSelected(true);
	}

	// 상점 슬롯 선택 해제
	if (CachedShopSlots.IsValidIndex(CurrentSelectedIndex) && CachedShopSlots[CurrentSelectedIndex])
	{
		CachedShopSlots[CurrentSelectedIndex]->SetSelected(false);
	}
	CurrentSelectedIndex = -1;
	CurrentSelectedPrice = 0;

	UpdateSellButtonState();
	UpdateBuyButtonState();
}

void UShop::UpdateBuyButtonState() const
{
	if (!MainBuyButton) return;

	if (CurrentSelectedIndex < 0)
	{
		MainBuyButton->SetIsEnabled(false);
		return;
	}

	if (AMyTestCharacter* PlayerChar = Cast<AMyTestCharacter>(GetOwningPlayerPawn()))
	{
		MainBuyButton->SetIsEnabled(PlayerChar->Coin >= CurrentSelectedPrice);
	}
	else
	{
		MainBuyButton->SetIsEnabled(false);
	}
}

void UShop::UpdateSellButtonState() const
{
	if (!MainSellButton) return;

	AMyTestCharacter* PlayerChar = Cast<AMyTestCharacter>(GetOwningPlayerPawn());
	const bool bCanSell = PlayerChar &&
		PlayerChar->InventoryComponent &&
		PlayerChar->InventoryComponent->InventoryItems.IsValidIndex(CurrentSelectedInventoryIndex);

	MainSellButton->SetIsEnabled(bCanSell);
}

void UShop::OnMainBuyButtonClicked()
{
	if (CurrentSelectedIndex < 0 || !CurrentShop) return;

	AMyTestCharacter* Buyer = Cast<AMyTestCharacter>(GetOwningPlayerPawn());
	if (!Buyer || !Buyer->InventoryComponent) return;

	const TArray<FShopItem>& Items = CurrentShop->GetItemsForSale();
	if (!Items.IsValidIndex(CurrentSelectedIndex)) return;
	if (CurrentShop->IsItemSoldOut(CurrentSelectedIndex)) return;

	const FShopItem& TargetItem = Items[CurrentSelectedIndex];
	if (!TargetItem.ItemData) return;

	const int32 FinalBuyPrice = TargetItem.GetBuyPrice();

	if (Buyer->Coin < FinalBuyPrice) return;
	if (Buyer->InventoryComponent->IsFull()) return;

	Buyer->Coin -= FinalBuyPrice;

	if (!Buyer->InventoryComponent->PurchaseItem(TargetItem))
	{
		Buyer->Coin += FinalBuyPrice;
		return;
	}

	CurrentShop->MarkItemSoldOut(CurrentSelectedIndex);

	if (CachedShopSlots.IsValidIndex(CurrentSelectedIndex) && CachedShopSlots[CurrentSelectedIndex])
	{
		CachedShopSlots[CurrentSelectedIndex]->SetSoldOut();
	}

	CurrentSelectedIndex = -1;
	CurrentSelectedPrice = 0;
	UpdateBuyButtonState();
}

void UShop::OnMainSellButtonClicked()
{
	AMyTestCharacter* Seller = Cast<AMyTestCharacter>(GetOwningPlayerPawn());
	if (!Seller || !Seller->InventoryComponent) return;

	if (!Seller->InventoryComponent->InventoryItems.IsValidIndex(CurrentSelectedInventoryIndex))
	{
		UpdateSellButtonState();
		return;
	}

	const FPartInstance TargetItem = Seller->InventoryComponent->InventoryItems[CurrentSelectedInventoryIndex];
	if (!TargetItem.SourceData) return;

	const int32 FinalSellPrice = ResolveSellPriceForPart(TargetItem.SourceData);

	if (!Seller->InventoryComponent->RemoveItem(TargetItem.InstanceID)) return;

	Seller->Coin += FinalSellPrice;

	CurrentSelectedInventoryIndex = -1;
	UpdateSellButtonState();
}

int32 UShop::ResolveSellPriceForPart(const UPartsData* PartData) const
{
    if (!PartData)
    {
        return 1;
    }

    if (CurrentShop)
    {
        const TArray<FShopItem>& Items = CurrentShop->GetItemsForSale();
        for (const FShopItem& ShopItem : Items)
        {
            if (ShopItem.ItemData == PartData)
            {
                return ShopItem.GetSellPrice();
            }
        }
    }

    if (PartData->DefaultSellPrice > 0)
    {
        return PartData->DefaultSellPrice;
    }

    return FMath::Max(1, PartData->DefaultBuyPrice / 2);
}

void UShop::OnShopRefreshClicked()
{
    if (!CurrentShop)
    {
        return;
    }

    CurrentShop->RebuildShopDataForTest();
    BuildShopSlotsOnce();
    RebindShopData();
}

void UShop::OnMainExitButtonClicked()
{
	CloseShop();
}

void UShop::CloseShop()
{
	ClearCurrentSelection();
	UnbindInventoryUpdatedDelegate();

	if (CurrentShop)
	{
		IShopInterface::Execute_CloseShop(CurrentShop);
		CurrentShop = nullptr;
	}
	else
	{
		RemoveFromParent();
	}
}
