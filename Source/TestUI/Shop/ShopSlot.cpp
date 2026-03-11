// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopSlot.h"
#include "Shop.h"
#include "../Datas/PartsData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../Datas/ShopItem.h"

void UShopSlot::NativeConstruct()
{
	Super::NativeConstruct();
	if (SlotSelectButton)
	{
		SlotSelectButton->OnClicked.AddDynamic(this, &UShopSlot::OnSlotSelected);
		SlotSelectButton->SetBackgroundColor(FLinearColor::Transparent);
	}
}

void UShopSlot::SetupSlot(const FShopItem& InItem, int32 InIndex, UShop* InParent)
{
	itemData = InItem;
	MainWidget = InParent;
	ItemIndex = InIndex;
	bIsSelected = false;
	SetAvailable();

	if (!InItem.ItemData)
	{
		SetEmpty();
		return;
	}

	if (ItemName) ItemName->SetText(InItem.ItemData->PartName);
	if (ItemPrice) ItemPrice->SetText(FText::AsNumber(InItem.GetBuyPrice()));

	if (PartTypeText)
	{
		FString TypeString = UEnum::GetDisplayValueAsText(InItem.ItemData->PartSlotType).ToString();
		PartTypeText->SetText(FText::FromString(TypeString));
	}

	if (ItemIcon && InItem.ItemData->PartIcon)
	{
		ItemIcon->SetBrushFromTexture(InItem.ItemData->PartIcon);
	}

	if (SlotSelectButton)
	{
		FLinearColor BgColor = FLinearColor::White;
		switch (InItem.ItemData->PartRarity)
		{
		case EItemRarity::Common:    BgColor = FLinearColor::Gray; break;
		case EItemRarity::Uncommon:  BgColor = FLinearColor::Green; break;
		case EItemRarity::Rare:      BgColor = FLinearColor(0.0f, 0.5f, 1.0f); break;
		case EItemRarity::Epic:      BgColor = FLinearColor(0.5f, 0.0f, 0.5f); break;
		case EItemRarity::Legendary: BgColor = FLinearColor(1.0f, 0.8f, 0.0f); break;
		}
		OriginalBackgroundColor = BgColor;
		
		// 배경색을 SlotBackground Image에 적용
		if (SlotBackground)
		{
			SlotBackground->SetColorAndOpacity(BgColor);
		}
	}
}

void UShopSlot::SetAvailable()
{
	bIsSoldOut = false;

	if (SlotSelectButton)
	{
		SlotSelectButton->SetIsEnabled(true);
	}

	if (ItemIcon)
	{
		ItemIcon->SetOpacity(1.0f);
		ItemIcon->SetColorAndOpacity(FLinearColor::White);
	}
}

void UShopSlot::SetSoldOut()
{
	bIsSoldOut = true;

	if (SlotSelectButton)
	{
		SlotSelectButton->SetIsEnabled(false);
	}

	if (ItemPrice)
	{
		ItemPrice->SetText(FText::FromString(TEXT("구매 완료")));
	}

	if (ItemIcon)
	{
		ItemIcon->SetOpacity(0.5f);
		ItemIcon->SetColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f));
	}
}

void UShopSlot::SetEmpty()
{
	bIsSoldOut = true;
	itemData = FShopItem();
	ItemIndex = INDEX_NONE;

	if (ItemName) ItemName->SetText(FText::GetEmpty());
	if (ItemPrice) ItemPrice->SetText(FText::GetEmpty());
	if (PartTypeText) PartTypeText->SetText(FText::GetEmpty());

	if (ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemIcon->SetOpacity(0.15f);
		ItemIcon->SetColorAndOpacity(FLinearColor::White);
	}

	if (SlotBackground)
	{
		SlotBackground->SetColorAndOpacity(FLinearColor(0.15f, 0.15f, 0.15f, 1.0f));
	}

	if (SlotSelectButton)
	{
		SlotSelectButton->SetIsEnabled(false);
	}
}

void UShopSlot::OnSlotSelected()
{
	if (bIsSoldOut)
	{
		return;
	}

	if (MainWidget)
	{
		MainWidget->SetSelectedItem(ItemIndex, itemData.GetBuyPrice());
	}
}

void UShopSlot::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;

	if (!SlotBackground) return;

	if (bSelected)
	{
		FLinearColor HighlightedColor = OriginalBackgroundColor * 1.8f;
		HighlightedColor.A = 1.0f;
		SlotBackground->SetColorAndOpacity(HighlightedColor);
	}
	else
	{
		SlotBackground->SetColorAndOpacity(OriginalBackgroundColor);
	}
}

