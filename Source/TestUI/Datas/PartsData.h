#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PartsData.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common      UMETA(DisplayName = "일반 (Common)"),
	Uncommon    UMETA(DisplayName = "고급 (Uncommon)"),
	Rare        UMETA(DisplayName = "희귀 (Rare)"),
	Epic        UMETA(DisplayName = "영웅 (Epic)"),
	Legendary   UMETA(DisplayName = "전설 (Legendary)")
};

UENUM(BlueprintType)
enum class EPartSlot : uint8
{
	None        UMETA(DisplayName = "없음"),
	Barrel      UMETA(DisplayName = "총열 (Barrel)"),
	Scope       UMETA(DisplayName = "조준경 (Scope)"),
	Magazine    UMETA(DisplayName = "탄창 (Magazine)"),
	Grip        UMETA(DisplayName = "손잡이 (Grip)"),
	Stock       UMETA(DisplayName = "개머리판 (Stock)")
};

USTRUCT(BlueprintType)
struct FPartInstance
{
	GENERATED_BODY()

	// 1. 원본 설계도 (데이터 에셋) 포인터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPartsData* SourceData = nullptr;

	// 2. 고유 ID (버리거나 팔 때 정확히 이 아이템을 찾기 위함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid InstanceID;

	// 생성자: 만들어질 때 자동으로 고유 ID 부여
	FPartInstance()
	{
		InstanceID = FGuid::NewGuid();
	}
};

struct FGunStats;

UCLASS(Abstract, BlueprintType)
class TESTUI_API UPartsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Info")
	FText PartName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Info")
	class UTexture2D* PartIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Info")
	EItemRarity PartRarity = EItemRarity::Common;

	// 파츠 기본 구매 가격
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Price", meta = (ClampMin = "1", UIMin = "1"))
	int32 DefaultBuyPrice = 100;

	// 파츠 기본 판매 가격
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Price", meta = (ClampMin = "0", UIMin = "0"))
	int32 DefaultSellPrice = 50;
    
	// 이 파츠가 장착될 부위
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Slot")
	EPartSlot PartSlotType = EPartSlot::None;
};
