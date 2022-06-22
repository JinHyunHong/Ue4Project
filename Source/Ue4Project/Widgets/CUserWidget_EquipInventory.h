#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_EquipInventory.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_EquipInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UUserWidget* GetEquipment() { return Equipment; }
	FORCEINLINE UUserWidget* GetInventory() { return Inventory; }

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UUserWidget* Equipment;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UUserWidget* Inventory;
};
