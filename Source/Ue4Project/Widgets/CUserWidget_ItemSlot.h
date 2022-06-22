#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CEquipCharacter.h"
#include "CUserWidget_ItemSlot.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class UCUserWidget_ToolTip> ToolTipClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot")
		FLinearColor BackColor;


protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Amount;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

protected:	
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent & InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	int32 GetAmount();
	
	void SetAmount(const int32& InAmount);
	void Update(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);
	void DeleteItem();

private:
	void UseItem();

public:
	FORCEINLINE const ECharacter& GetCharacterType() { return CharacterType; }
	FORCEINLINE const EEquipmentType& GetEquipmentType() { return EquipType; }
	FORCEINLINE const FString& GetItemName() { return ItemName; }
	FORCEINLINE const FItem& GetItemData() { return ItemData; }

protected:
	ECharacter CharacterType;
	EEquipmentType EquipType;
	FString ItemName;
	FItem ItemData;

private:
	class UCUserWidget_ToolTip* ToolTip;
	bool bEnter; // 마우스가 위젯에 Enter 했는지
};
