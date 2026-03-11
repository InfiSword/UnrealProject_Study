#include "DataManager.h"
#include "PartsData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"

void UDataManager::EnsurePartsReady()
{
	if (bPartsInitialized)
	{
		return;
	}

	bPartsInitialized = true;

	if (RegisteredParts.Num() <= 0)
	{
		RefreshPartsFromAssetRegistry();
	}
}

void UDataManager::RefreshPartsFromAssetRegistry()
{
	RegisteredParts.Reset();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	FARFilter Filter;
	Filter.ClassPaths.Add(UPartsData::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	
	// /Game/Data/Parts 폴더 하위에서만 검색
	Filter.PackagePaths.Add(FName("/Game/Data/Parts"));

	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	for (const FAssetData& AssetData : AssetDataList)
	{
		UPartsData* PartData = Cast<UPartsData>(AssetData.GetAsset());
		if (PartData)
		{
			RegisteredParts.AddUnique(PartData);
			UE_LOG(LogTemp, Log, TEXT("DataManagerSubsystem: 파츠 로드 - %s (희귀도: %d)"), *PartData->GetName(), static_cast<int32>(PartData->PartRarity));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("DataManagerSubsystem: 총 파츠 %d개 로드 완료"), RegisteredParts.Num());
}

void UDataManager::RegisterPart(UPartsData* PartData)
{
	if (!PartData)
	{
		return;
	}

	RegisteredParts.AddUnique(PartData);
	bPartsInitialized = true;
}

void UDataManager::ClearRegisteredParts()
{
	RegisteredParts.Reset();
	bPartsInitialized = false;
}

TArray<FShopItem> UDataManager::BuildRandomShopItems(int32 RequestedCount)
{
	TArray<FShopItem> Result;

	if (RequestedCount <= 0)
	{
		return Result;
	}

	EnsurePartsReady();

	TArray<UPartsData*> CandidateParts;
	CandidateParts.Reserve(RegisteredParts.Num());

	for (UPartsData* Part : RegisteredParts)
	{
		if (Part)
		{
			CandidateParts.Add(Part);
		}
	}

	if (CandidateParts.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataManagerSubsystem: 등록된 파츠가 없어 상점 목록을 생성하지 못했습니다."));
		return Result;
	}

	const int32 FinalCount = FMath::Min(RequestedCount, CandidateParts.Num());
	Result.Reserve(FinalCount);

	for (int32 PickCount = 0; PickCount < FinalCount; ++PickCount)
	{
		float TotalWeight = 0.0f;
		for (UPartsData* Part : CandidateParts)
		{
			TotalWeight += FMath::Max(0.0f, GetRarityWeight(Part));
		}

		int32 ChosenIndex = INDEX_NONE;
		if (TotalWeight > 0.0f)
		{
			float Roll = FMath::FRandRange(0.0f, TotalWeight);
			for (int32 i = 0; i < CandidateParts.Num(); ++i)
			{
				Roll -= FMath::Max(0.0f, GetRarityWeight(CandidateParts[i]));
				if (Roll <= 0.0f)
				{
					ChosenIndex = i;
					break;
				}
			}
		}

		if (!CandidateParts.IsValidIndex(ChosenIndex))
		{
			ChosenIndex = FMath::RandRange(0, CandidateParts.Num() - 1);
		}

		FShopItem NewItem;
		NewItem.ItemData = CandidateParts[ChosenIndex];
		Result.Add(NewItem);

		// 중복 방지를 위해 뽑은 항목은 후보에서 제거
		CandidateParts.RemoveAtSwap(ChosenIndex);
	}

	return Result;
}

float UDataManager::GetRarityWeight(const UPartsData* PartData) const
{
	if (!PartData)
	{
		return 1.0f;
	}

	switch (PartData->PartRarity)
	{
	case EItemRarity::Common:
		return 60.0f;
	case EItemRarity::Uncommon:
		return 25.0f;
	case EItemRarity::Rare:
		return 10.0f;
	case EItemRarity::Epic:
		return 4.0f;
	case EItemRarity::Legendary:
		return 1.0f;
	default:
		return 1.0f;
	}
}
