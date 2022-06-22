#include "Widgets/CUserWidget_Inventory.h"
#include "Global.h"
#include "Widgets/CUserWidget_ItemSlot.h"
#include "GameFramework/PlayerController.h"
#include "Characters/CEquipCharacter.h"
#include "Components/CInventoryComponent.h"
#include "Components/GridPanel.h"

void UCUserWidget_Inventory::NativeConstruct()
{
	TArray<UWidget*> widgets = Grid->GetAllChildren();

	for (UWidget* widget : widgets)
	{
		UCUserWidget_ItemSlot* data = Cast<UCUserWidget_ItemSlot>(widget);
		if (!!data) Datas.Add(data);
	}

	Super::NativeConstruct();
}

UCUserWidget_Inventory::UCUserWidget_Inventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UCUserWidget_Inventory::OnExitPress()
{
	Super::OnExitPress();

	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwningPlayer()->GetPawn());
	CheckNull(owner);

	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(owner);
	CheckNull(inventory);

	inventory->ViewInventory();
}

void UCUserWidget_Inventory::AddItem(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	// Index가 Slot개수보다 크다면 꽉찬 것으로 간주
	CheckTrue(Index > Grid->GetChildrenCount());

	FindIndex(InItemName, InItemData);
	
	Datas[Index]->Update(InCharacterType, InEquipType, InItemName, InItemData);
}

void UCUserWidget_Inventory::FindIndex(const FString& InItemName, const FItem& InItemData)
{
	for (int32 i = 0; i < Datas.Num() - 1; i++)
	{
		bool bStack = InItemData.bStack;
		bStack &= Datas[i]->GetItemName() == InItemName;
		
		if (bStack)
		{
			Datas[i]->SetAmount(Datas[i]->GetAmount() + 1);

			return;
		}

		else
		{
			if (Datas[i]->GetItemName() == "")
			{
				Index = i;

				return;
			}
		}
	}
}

bool UCUserWidget_Inventory::IsValidSlot(uint32 InIndex)
{
	return Datas[InIndex]->GetItemName() != "";
}

