#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CEquipCharacter.h"
#include "CUserWidget_EquipSlot.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_EquipSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class UCUserWidget_ToolTip> ToolTipClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot")
		class UTexture2D* BackImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot")
		FLinearColor BackColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.3f);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemImage;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

protected:
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


public:
	void Update(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData, bool bInitEquip);
	void DeleteItem();

private:
	void UnEquipItem();

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
