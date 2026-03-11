// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "InventoryMainWidget.h"
#include "../Shop/Shop.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotSelectButton)
	{
		SlotSelectButton->OnClicked.AddDynamic(this, &UInventorySlot::OnSlotSelected);
		
		// 버튼을 투명하게 설정 (배경색은 SlotBackground Image에서 관리)
		SlotSelectButton->SetBackgroundColor(FLinearColor::Transparent);
	}
}

void UInventorySlot::SetupInvenSlot(const FPartInstance& InInstance, int32 InIndex, bool bEquipped, UShop* InShopMainWidget)
{
	ItemInstance = InInstance;
	ItemIndex = InIndex;
	bIsEquipped = bEquipped;
	ShopMainWidget = InShopMainWidget;
	bIsSelected = false;

	if (!InInstance.SourceData)
	{
		return;
	}

	if (ItemName)
	{
		ItemName->SetText(InInstance.SourceData->PartName);
	}

	if (PartTypeText)
	{
		const FString TypeString = UEnum::GetDisplayValueAsText(InInstance.SourceData->PartSlotType).ToString();
		PartTypeText->SetText(FText::FromString(TypeString));
	}

	if (ItemIcon && InInstance.SourceData->PartIcon)
	{
		ItemIcon->SetBrushFromTexture(InInstance.SourceData->PartIcon);
	}

	if (EquipStatusText)
	{
		if (bIsEquipped)
		{
			EquipStatusText->SetText(FText::FromString(TEXT("[장착중]")));
			EquipStatusText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
		}
		else
		{
			EquipStatusText->SetText(FText::GetEmpty());
		}
	}

	if (SlotSelectButton)
	{
		FLinearColor BgColor = FLinearColor::White;
		const float Brightness = bIsEquipped ? 1.2f : 1.0f;

		switch (InInstance.SourceData->PartRarity)
		{
		case EItemRarity::Common:
			BgColor = FLinearColor::Gray * Brightness;
			break;
		case EItemRarity::Uncommon:
			BgColor = FLinearColor::Green * Brightness;
			break;
		case EItemRarity::Rare:
			BgColor = FLinearColor(0.0f, 0.5f, 1.0f) * Brightness;
			break;
		case EItemRarity::Epic:
			BgColor = FLinearColor(0.5f, 0.0f, 0.5f) * Brightness;
			break;
		case EItemRarity::Legendary:
			BgColor = FLinearColor(1.0f, 0.8f, 0.0f) * Brightness;
			break;
		default:
			break;
		}

		OriginalBackgroundColor = BgColor;
		
		// 배경색을 SlotBackground Image에 적용
		if (SlotBackground)
		{
			SlotBackground->SetColorAndOpacity(BgColor);
		}
	}
}

void UInventorySlot::OnSlotSelected()
{
	if (ShopMainWidget)
	{
		ShopMainWidget->SetSelectedInventoryItem(ItemIndex);
	}

	if (ItemInstance.SourceData)
	{
		UE_LOG(LogTemp, Log, TEXT("인벤토리 슬롯 선택됨: %s (인덱스: %d, 장착 상태: %s)"),
			*ItemInstance.SourceData->PartName.ToString(),
			ItemIndex,
			bIsEquipped ? TEXT("장착중") : TEXT("미장착"));
	}
}

void UInventorySlot::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;

	if (!SlotBackground) return;

	if (bSelected)
	{
		// 선택 시: 배경색을 밝게 하고 강조 효과 추가
		FLinearColor HighlightedColor = OriginalBackgroundColor * 1.8f;
		HighlightedColor.A = 1.0f;
		SlotBackground->SetColorAndOpacity(HighlightedColor);
	}
	else
	{
		// 선택 해제 시: 원래 배경색으로 복원
		SlotBackground->SetColorAndOpacity(OriginalBackgroundColor);
	}
}

