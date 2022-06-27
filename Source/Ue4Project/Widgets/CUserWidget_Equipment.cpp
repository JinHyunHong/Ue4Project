#include "Widgets/CUserWidget_Equipment.h"
#include "Global.h"
#include "Widgets/CUserWidget_EquipSlot.h"
#include "GameFramework/PlayerController.h"
#include "Characters/CEquipCharacter.h"
#include "Components/CInventoryComponent.h"
#include "Components/GridPanel.h"

void UCUserWidget_Equipment::NativeConstruct()
{
	TArray<UCUserWidget_EquipSlot*> slots;

	TArray<UWidget*> widgets = GridLeft->GetAllChildren();

	for (UWidget* widget : widgets)
	{
		UCUserWidget_EquipSlot* slot = Cast<UCUserWidget_EquipSlot>(widget);
		if (!!slot) slots.Add(slot);
	}

	widgets = GridRight->GetAllChildren();

	for (UWidget* widget : widgets)
	{
		UCUserWidget_EquipSlot* slot = Cast<UCUserWidget_EquipSlot>(widget);
		if (!!slot) slots.Add(slot);
	}

	for (UCUserWidget_EquipSlot* slot : slots)
	{
		EEquipmentType type = CHelpers::GetStringToEnum<EEquipmentType>("EEquipmentType", slot->GetName());
		Datas.Add(type, slot);
	}

	Super::NativeConstruct();
}

UCUserWidget_Equipment::UCUserWidget_Equipment(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}


void UCUserWidget_Equipment::OnExitPress()
{
	Super::OnExitPress();

	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwningPlayer()->GetPawn());
	CheckNull(owner);

	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(owner);
	CheckNull(inventory);

	inventory->ViewEquipment();
}



void UCUserWidget_Equipment::AddEquip(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	UCUserWidget_EquipSlot* slot = Datas[InEquipType];
	slot->Update(InCharacterType, InEquipType, InItemName, InItemData);
}

bool UCUserWidget_Equipment::IsValidSlot(const EEquipmentType& InEquipType)
{
	return Datas[InEquipType]->GetItemName() != "";
}
