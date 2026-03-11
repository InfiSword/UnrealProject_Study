// Copyright (c) 2024, Your Name or Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IShopInterface.generated.h"

class AMyTestCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class UShopInterface : public UInterface
{
	GENERATED_BODY()
};

class TESTUI_API IShopInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shop")
	bool OpenShop(AMyTestCharacter* Player);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shop")
	void CloseShop();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shop")
	bool IsShopOpen() const;
};
