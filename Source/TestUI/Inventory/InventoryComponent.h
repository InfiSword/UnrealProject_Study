// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FPartInstance;
struct FShopItem;
class UPartsData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTUI_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FPartInstance> InventoryItems;

	// 가방의 최대 칸 수 (기본값 30칸)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxCapacity = 30;

	// 아이템이 추가되거나 삭제될 때 UI를 갱신하라고 알리는 방송국
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Events")
	FOnInventoryUpdated OnInventoryUpdated;

	// --------------------------------------------------
	// [기능 함수]
	// --------------------------------------------------

	// 가방이 꽉 찼는지 확인 (상점 결제 전에 호출됨)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool IsFull() const;

	// 아이템 획득 (상점에서 UPartsData 원본을 넘겨주면, 여기서 내 소유로 만듭니다)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(class UPartsData* NewItemData);

	// 아이템 삭제 (나중에 아이템을 팔거나 버릴 때 사용)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FGuid ItemID);

	// 상점에서 선택한 아이템을 구매하여 인벤토리에 추가
	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool PurchaseItem(const FShopItem& SelectedItem);
};
