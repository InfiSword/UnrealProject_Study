// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Datas/ShopItem.h"
#include "ShopActor.generated.h"

class AMyTestCharacter;

UCLASS()
class TESTUI_API AShopActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopActor();
	
	// 이 상점에서 파는 물건들의 목록 (에디터에서 맘대로 세팅 가능!)
	UPROPERTY(EditAnywhere, Category = "Shop")
	TArray<FShopItem> ItemsForSale;

	// ★ 핵심: 구매 처리 함수
	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool BuyItem(AMyTestCharacter* Buyer, int32 ItemIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
