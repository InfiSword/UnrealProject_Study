#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GunStats.h"
#include "PartsData.generated.h"

USTRUCT(BlueprintType)
struct FPartInstance
{
	GENERATED_BODY()

	// 1. 원본 설계도 (데이터 에셋) 포인터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPartsData* SourceData = nullptr;

	// 2. 현재 강화 레벨 (변하는 값)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 1;

	// 3. 고유 ID (버리거나 팔 때 정확히 이 아이템을 찾기 위함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid InstanceID;

	// 생성자: 만들어질 때 자동으로 고유 ID 부여
	FPartInstance()
	{
		InstanceID = FGuid::NewGuid();
	}
};

UCLASS(Abstract, BlueprintType)
class TESTUI_API UPartsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// ★ [추가된 부분] 에셋 매니저가 이 파츠를 인식하게 해주는 필수 함수
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// "PartItem"이라는 카테고리로, 이 에셋의 파일 이름(GetFName)을 ID로 사용합니다.
		return FPrimaryAssetId(FPrimaryAssetType("PartItem"), GetFName());
	}
	
	// 공통 정보 (이름, 아이콘 등)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Info")
	FText PartName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Info")
	class UTexture2D* PartIcon;

	// ★ 핵심 함수: 자식 클래스들이 각자 방식대로 총의 스탯을 개조할 수 있게 함
	virtual void ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData)
	const PURE_VIRTUAL(UPartsData::ApplyStatsToGun, );
};
