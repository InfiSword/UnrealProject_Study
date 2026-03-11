#include "InventoryComponent.h" 
#include "../Datas/PartsData.h"
#include "../Datas/ShopItem.h"

UInventoryComponent::UInventoryComponent()
{
	// 인벤토리는 매 프레임(Tick)마다 검사할 필요가 없으므로 꺼둡니다 (성능 최적화)
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::IsFull() const
{
	return InventoryItems.Num() >= MaxCapacity;
}

bool UInventoryComponent::AddItem(UPartsData* NewItemData)
{
	if (IsFull() || !NewItemData)
	{
		return false;
	}

	// 1. 내 가방에 넣을 새로운 '인스턴스(나만의 아이템)' 생성
	FPartInstance NewPart;
	NewPart.SourceData = NewItemData;

	// 2. 가방 배열에 추가
	InventoryItems.Add(NewPart);

	// 3. UI 갱신 방송
	OnInventoryUpdated.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("인벤토리에 아이템 추가됨: %s"), *NewItemData->PartName.ToString());
	return true;
}

bool UInventoryComponent::RemoveItem(FGuid ItemID)
{
	// 람다(Lambda)식을 이용해서, ID가 일치하는 아이템을 배열에서 모두 지웁니다.
	int32 RemovedCount = InventoryItems.RemoveAll([&](const FPartInstance& Part) {
		return Part.InstanceID == ItemID;
	});

	if (RemovedCount > 0)
	{
		OnInventoryUpdated.Broadcast();
		return true;
	}
    
	return false;
}

bool UInventoryComponent::PurchaseItem(const FShopItem& SelectedItem)
{
	// 1. 유효성 검사
	if (!SelectedItem.ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("PurchaseItem: 아이템 데이터가 없습니다."));
		return false;
	}

	// 2. 인벤토리가 꽉 찼는지 확인
	if (IsFull())
	{
		UE_LOG(LogTemp, Warning, TEXT("PurchaseItem: 인벤토리가 꽉 찼습니다."));
		return false;
	}

	// 3. FShopItem의 ItemData를 기반으로 새로운 FPartInstance 생성
	FPartInstance NewPart;
	NewPart.SourceData = SelectedItem.ItemData;
	// InstanceID는 FPartInstance 생성자에서 자동으로 생성됨

	// 4. InventoryItems 배열에 추가
	InventoryItems.Add(NewPart);

	// 5. UI 갱신 방송
	OnInventoryUpdated.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("PurchaseItem 성공: %s"),
		*SelectedItem.ItemData->PartName.ToString());

	return true;
}
