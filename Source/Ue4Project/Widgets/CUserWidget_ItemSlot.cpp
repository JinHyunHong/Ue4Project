#include "Widgets/CUserWidget_ItemSlot.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CEquipComponent.h"
#include "Components/CInventoryComponent.h"
#include "Widgets/CUserWidget_Equipment.h"
#include "Widgets/CUserWidget_EquipSlot.h"
#include "Widgets/CUserWidget_ToolTip.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UCUserWidget_ItemSlot::NativePreConstruct()
{
	Amount->SetVisibility(ESlateVisibility::Hidden);
	ItemImage->SetColorAndOpacity(BackColor);

	Super::NativePreConstruct();
}

void UCUserWidget_ItemSlot::NativeOnInitialized()
{
	ToolTip = CreateWidget<UCUserWidget_ToolTip, APlayerController>(GetOwningPlayer(), ToolTipClass);
	ToolTip->SetVisibility(ESlateVisibility::Collapsed);
	ToolTip->AddToViewport(10);

	Super::NativeOnInitialized();
}

FReply UCUserWidget_ItemSlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmptyResult(ItemName, FReply::Unhandled());
	CheckNullResult(ItemData.Mesh, FReply::Unhandled());
	CheckNullResult(ToolTip, FReply::Unhandled());

	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bEnter)
	{
		FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		float scale = UWidgetLayoutLibrary::GetViewportScale(this->GetWorld());
		mousePos *= scale;

		ToolTip->SetPositionInViewport(mousePos);
	}
	
	return FReply::Handled();
}

void UCUserWidget_ItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bEnter = true;

	CheckEmpty(ItemName);
	CheckNull(ItemData.Mesh);

	CheckNull(ToolTip);
	CheckTrue(ToolTip->GetVisibility() == ESlateVisibility::HitTestInvisible);
	ToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCUserWidget_ItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bEnter = false;

	CheckEmpty(ItemName);
	CheckNull(ItemData.Mesh);

	CheckNull(ToolTip);
	CheckTrue(ToolTip->GetVisibility() == ESlateVisibility::Collapsed);
	ToolTip->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UCUserWidget_ItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmptyResult(ItemName, FReply::Unhandled());
	CheckNullResult(ItemData.Mesh, FReply::Unhandled());
	CheckNullResult(ToolTip, FReply::Unhandled());

	CheckFalseResult(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton), FReply::Unhandled());

	// ItemUse
	UseItem();
		
	return FReply::Handled();
}

int32 UCUserWidget_ItemSlot::GetAmount()
{
	FString amount = Amount->GetText().ToString();
	return FCString::Atoi(*amount);
}

void UCUserWidget_ItemSlot::SetAmount(const int32& InAmount)
{
	Amount->SetText(FText::FromString(FString::FromInt(InAmount)));
}

void UCUserWidget_ItemSlot::Update(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	CheckEmpty(InItemName);

	CharacterType = InCharacterType;
	EquipType = InEquipType;
	ItemName = InItemName;
	ItemData = InItemData;

	ItemImage->SetBrushFromTexture(ItemData.SumNail);
	ItemImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	ESlateVisibility view;

	// None -> Item
	InEquipType != EEquipmentType::None ? view = ESlateVisibility::Hidden : view = ESlateVisibility::Visible;
	Amount->SetVisibility(view);

	CheckNull(ToolTip);
	ToolTip->Update(ItemName, ItemData);
}

void UCUserWidget_ItemSlot::DeleteItem()
{
	CharacterType = ECharacter::Man;
	EquipType = EEquipmentType::None;
	ItemName = "";
	ItemData = FItem();
	bEnter = false;

	ItemImage->SetColorAndOpacity(BackColor);

	ToolTip->SetVisibility(ESlateVisibility::Hidden);
}

void UCUserWidget_ItemSlot::UseItem()
{
	CheckEmpty(ItemName);

	ACPlayer* owner = Cast<ACPlayer>(GetOwningPlayer()->GetPawn());
	CheckNull(owner);

	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(owner);
	CheckNull(inventory);

	switch (EquipType)
	{
		case EEquipmentType::None:
		{
			// TODO : Item

			break;
		}
		default:
		{
			UCEquipComponent* equip = CHelpers::GetComponent<UCEquipComponent>(owner);
			CheckNull(equip);

			UCUserWidget_Equipment* equipWidget = inventory->GetEquipment();
			CheckNull(equipWidget);

			// 바꾸려는 EquipSlot에 아이템이 존재할 경우 Swap
			if (equipWidget->IsValidSlot(EquipType))
			{
				EEquipmentType typeTemp = EquipType;
				FString nameTemp = ItemName;
				FItem itemTemp = ItemData;

				UCUserWidget_EquipSlot* equipSlot = equipWidget->GetSlot(EquipType); 
				Update(equipSlot->GetCharacterType(), equipSlot->GetEquipmentType(), equipSlot->GetItemName(), equipSlot->GetItemData());
				
				equip->Equip(typeTemp, nameTemp, itemTemp);
				
				return;
			}
			
			equip->Equip(EquipType, ItemName, ItemData);
			break;
		}
	}

	DeleteItem();
}
