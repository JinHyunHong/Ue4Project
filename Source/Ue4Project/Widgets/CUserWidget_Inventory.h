#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CUserWidget_Window.h"
#include "Characters/CEquipCharacter.h"
#include "CUserWidget_Inventory.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_Inventory : public UCUserWidget_Window
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* Grid;

public:
	UCUserWidget_Inventory(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	void OnExitPress() override;

public:
	void AddItem(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);

private:
	void FindIndex(const FString& InItemName, const FItem& InItemData);

public:
	FORCEINLINE class UCUserWidget_ItemSlot* GetSlot(uint32 InIndex) { return Datas[InIndex]; }
	bool IsValidSlot(uint32 InIndex);

protected:
	int32 Index = 0;
	TArray<class UCUserWidget_ItemSlot*> Datas;
};
