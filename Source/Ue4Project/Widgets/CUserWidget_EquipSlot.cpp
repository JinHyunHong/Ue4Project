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
	// ZOrder를 높여서 최상위로 ToolTip이 보이도록 함
	ToolTip->AddToViewport(10);

	Super::NativeOnInitialized();
}

FReply UCUserWidget_EquipSlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UCUserWidget_EquipSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bEnter = true;

	CheckEmpty(ItemName);
	CheckNull(ItemData.Mesh);

	CheckNull(ToolTip);
	CheckTrue(ToolTip->GetVisibility() == ESlateVisibility::HitTestInvisible);
	ToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCUserWidget_EquipSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bEnter = false;

	CheckEmpty(ItemName);
	CheckNull(ItemData.Mesh);

	CheckNull(ToolTip);
	CheckTrue(ToolTip->GetVisibility() == ESlateVisibility::Collapsed);
	ToolTip->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UCUserWidget_EquipSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CheckEmptyResult(ItemName, FReply::Unhandled());
	CheckNullResult(ItemData.Mesh, FReply::Unhandled());
	CheckNullResult(ToolTip, FReply::Unhandled());

	CheckFalseResult(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton), FReply::Unhandled());

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
	CharacterType = ECharacter::Man;
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
	CheckNull(ItemData.Mesh);

	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwningPlayer()->GetPawn());
	CheckNull(owner);

	UCEquipComponent* equip = CHelpers::GetComponent<UCEquipComponent>(owner);
	CheckNull(equip);

	equip->Unequip(EquipType, ItemName, ItemData);
	DeleteItem();
}

