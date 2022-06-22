#include "Widgets/CUserWidget_ItemSlot.h"
#include "Global.h"
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
	Super::NativeOnInitialized();

	ToolTip = CreateWidget<UCUserWidget_ToolTip, APlayerController>(GetOwningPlayer(), ToolTipClass);
	ToolTip->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UCUserWidget_ItemSlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmptyResult(ItemName, FReply::Unhandled());
	CheckNullResult(ItemData.SumNail, FReply::Unhandled());
	CheckNullResult(ToolTip, FReply::Unhandled());

	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bEnter)
	{
		if (ToolTip->IsInViewport() == false) ToolTip->AddToViewport(0);

		FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		float scale = UWidgetLayoutLibrary::GetViewportScale(this->GetWorld());
		mousePos *= scale;

		ToolTip->SetPositionInViewport(mousePos);
	}
	
	return FReply::Handled();
}

void UCUserWidget_ItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmpty(ItemName);
	CheckNull(ItemData.SumNail);
	CheckNull(ToolTip);
	CheckFalse(ToolTip->GetVisibility() != ESlateVisibility::HitTestInvisible);

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	ToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);

	bEnter = true;	
}

void UCUserWidget_ItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	CheckEmpty(ItemName);
	CheckNull(ItemData.SumNail);
	CheckNull(ToolTip);
	CheckFalse(ToolTip->GetVisibility() != ESlateVisibility::Collapsed);

	Super::NativeOnMouseLeave(InMouseEvent);

	bEnter = false;

	ToolTip->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UCUserWidget_ItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmptyResult(ItemName, FReply::Unhandled());
	CheckNullResult(ItemData.SumNail, FReply::Unhandled());
	CheckNullResult(ToolTip, FReply::Unhandled());

	CheckFalseResult(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton), FReply::Unhandled());

	// ItemUse
	CLog::Print("RightClick");
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

	ToolTip->Update(ItemName, ItemData);
}

void UCUserWidget_ItemSlot::DeleteItem()
{
	CharacterType = ECharacter::Sword;
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

	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwningPlayer()->GetPawn());
	CheckNull(owner);

	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(owner);
	CheckNull(inventory);

	UCUserWidget_Equipment* equipWidget = inventory->GetEquipment();
	CheckNull(equipWidget);

	switch (EquipType)
	{
		case EEquipmentType::None:
		{
			// TODO : Item

			break;
		}
		default:
		{
			// 바꾸려는 EquipSlot에 아이템이 존재할 경우 Swap
			if (equipWidget->IsValidSlot(EquipType))
			{
				UCUserWidget_EquipSlot* equipSlot = equipWidget->GetSlot(EquipType);
				Update(equipSlot->GetCharacterType(), equipSlot->GetEquipmentType(), equipSlot->GetItemName(), equipSlot->GetItemData());
				inventory->AddEquip(CharacterType, EquipType, ItemName, ItemData);
				return;
			}

			inventory->AddEquip(CharacterType, EquipType, ItemName, ItemData);
			break;
		}
	}

	DeleteItem();
}
