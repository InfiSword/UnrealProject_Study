// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopActor.h"
#include "../Inventory/InventoryComponent.h"
#include "../Player/MyTestCharacter.h"

// Sets default values
AShopActor::AShopActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AShopActor::BuyItem(AMyTestCharacter* Buyer, int32 ItemIndex)
{
	// 1. 방어 코드: 구매자가 없거나, 인덱스가 범위를 벗어나면 취소
	if (!Buyer || !ItemsForSale.IsValidIndex(ItemIndex))
	{
		return false;
	}

	// 2. 선택한 상품 정보 가져오기
	FShopItem& TargetItem = ItemsForSale[ItemIndex];
	if (!TargetItem.ItemData)
	{
		return false;
	}

	// 3. 구매자의 가방(인벤토리 컴포넌트) 찾기
	// FindComponentByClass를 쓰면 캐릭터 헤더에 변수가 어떻게 선언되어 있든 안전하게 찾아옵니다.
	// ※ UInventoryComponent 부분은 본인이 만든 인벤토리 컴포넌트 클래스명으로 바꿔주세요! (예: UPartInventoryComponent)
	UInventoryComponent* BuyerInven = Buyer->FindComponentByClass<UInventoryComponent>();
    
	if (!BuyerInven)
	{
		UE_LOG(LogTemp, Warning, TEXT("구매자에게 인벤토리 컴포넌트가 없습니다!"));
		return false;
	}

	// 4. 결제 진행 로직 (예시: 여기서는 무조건 성공한다고 가정하거나 본인의 PlayerState 로직 연결)
	bool bPurchaseSuccess = true; // 나중에 실제 돈 차감 로직으로 변경하세요.
    
	if (bPurchaseSuccess)
	{
		// 5. 인벤토리에 아이템 추가
		// ※ AddItem 함수는 본인이 인벤토리 컴포넌트에 만든 함수 이름에 맞춰주세요.
		// BuyerInven->AddItem(TargetItem.ItemData);
        
		UE_LOG(LogTemp, Log, TEXT("아이템 구매 완료!"));
		return true;
	}

	return false;
}

// Called when the game starts or when spawned
void AShopActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShopActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

