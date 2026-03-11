#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MyTestCharacter.generated.h"

class UInventoryComponent;

UCLASS()
class TESTUI_API AMyTestCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMyTestCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency")
	int32 Coin = 1000;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnInteractKeyPressed();

protected:
	virtual void BeginPlay() override;
};
