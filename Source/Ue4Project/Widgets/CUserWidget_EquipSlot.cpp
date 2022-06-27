#include "Widgets/CUserWidget_EquipSlot.h"
#include "Global.h"
#include "Components/CEquipComponent.h"
#include "Components/CInventoryComponent.h"
#include "Components/Image.h"
#include "Widgets/CUserWidget_ToolTip.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UCUserWidget_EquipSlot::NativePreConstruct()
{
	ItemImage->SetColorAndOpacity(BackColor);

	if(!!BackImage)
		ItemImage->SetBrushFromTexture(BackImage);

	Super::NativePreConstruct();
}

void UCUserWidget_EquipSlot::NativeOnInitialized()
{
	ToolTip = CreateWidget<UCUserWidget_ToolTip, APlayerController>(GetOwningPlayer(), ToolTipClass);
	ToolTip->SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeOnInitialized();
}

FReply UCUserWidget_EquipSlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmptyResult(ItemName, FReply::Unhandled());
	CheckEmptyResult(ItemData.Desc, FReply::Unhandled());
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

void UCUserWidget_EquipSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckNull(ToolTip);
	CheckEmpty(ItemName);
	CheckEmpty(ItemData.Desc);
	CheckFalse(ToolTip->GetVisibility() != ESlateVisibility::HitTestInvisible);

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	ToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
	bEnter = true;
}

void UCUserWidget_EquipSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	CheckNull(ToolTip);
	CheckEmpty(ItemName);
	CheckEmpty(ItemData.Desc);
	CheckFalse(ToolTip->GetVisibility() != ESlateVisibility::Collapsed);

	Super::NativeOnMouseLeave(InMouseEvent);

	bEnter = false;

	ToolTip->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UCUserWidget_EquipSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmptyResult(ItemName, FReply::Unhandled());
	CheckNullResult(ItemData.SumNail, FReply::Unhandled());
	CheckNullResult(ToolTip, FReply::Unhandled());

	CheckFalseResult(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton), FReply::Unhandled());

	CLog::Print("RightClick");
	UnEquipItem();

	return FReply::Handled();
}

void UCUserWidget_EquipSlot::Update(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	CheckEmpty(InItemName);
	CheckTrue(InEquipType == EEquipmentType::None);
	CheckEmpty(InItemData.Desc);

	CharacterType = InCharacterType;
	EquipType = InEquipType;
	ItemName = InItemName;
	ItemData = InItemData;

	ItemImage->SetBrushFromTexture(ItemData.SumNail);
	ItemImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	CheckNull(ToolTip);
	ToolTip->Update(ItemName, ItemData);
}

void UCUserWidget_EquipSlot::DeleteItem()
{
	CharacterType = ECharacter::Sword;
	EquipType = EEquipmentType::None;
	ItemName = "";
	ItemData = FItem();
	bEnter = false;

	ItemImage->SetColorAndOpacity(BackColor);

	CheckNull(ToolTip);
	ToolTip->SetVisibility(ESlateVisibility::Hidden);
}

void UCUserWidget_EquipSlot::UnEquipItem()
{
	CheckEmpty(ItemName);

	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwningPlayer()->GetPawn());
	CheckNull(owner);

	UCEquipComponent* equip = CHelpers::GetComponent<UCEquipComponent>(owner);
	CheckNull(equip);

	equip->Unequip(EquipType, ItemName, ItemData);
	DeleteItem();
}

