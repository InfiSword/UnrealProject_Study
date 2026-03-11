#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShopItem.h"
#include "DataManager.generated.h"

class UPartsData;

UCLASS()
class TESTUI_API UDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 수동 등록된 파츠 풀(런타임에도 BP로 추가 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<UPartsData>> RegisteredParts;

	// 파츠 풀이 비어 있으면 AssetRegistry에서 UPartsData를 자동 수집
	UFUNCTION(BlueprintCallable, Category = "Data")
	void RefreshPartsFromAssetRegistry();

	// 필요 시 외부에서 파츠를 추가 등록
	UFUNCTION(BlueprintCallable, Category = "Data")
	void RegisterPart(UPartsData* PartData);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void ClearRegisteredParts();

	// 등록된 파츠에서 랜덤으로 RequestedCount개를 뽑아 상점 아이템 생성
	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FShopItem> BuildRandomShopItems(int32 RequestedCount);

private:
	bool bPartsInitialized = false;
	void EnsurePartsReady();
	float GetRarityWeight(const UPartsData* PartData) const;
};