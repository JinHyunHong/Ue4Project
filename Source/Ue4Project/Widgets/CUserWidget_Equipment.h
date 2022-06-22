#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CUserWidget_Window.h"
#include "Characters/CEquipCharacter.h"
#include "CUserWidget_Equipment.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_Equipment : public UCUserWidget_Window
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* GridLeft;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* GridRight;

public:
	UCUserWidget_Equipment(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	void OnExitPress() override;

public:
	void AddEquip(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData, bool bInitEquip);

public:
	FORCEINLINE class UCUserWidget_EquipSlot* GetSlot(const EEquipmentType& InEquipType) { return Datas[InEquipType]; }
	bool IsValidSlot(const EEquipmentType& InEquipType);

protected:
	TMap<EEquipmentType, class UCUserWidget_EquipSlot*> Datas;
};
