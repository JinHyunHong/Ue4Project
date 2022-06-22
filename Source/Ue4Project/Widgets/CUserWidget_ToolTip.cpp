#include "Widgets/CUserWidget_ToolTip.h"
#include "Global.h"
#include "Components/TextBlock.h"

void UCUserWidget_ToolTip::Update(FString& InItemName, FItem& InItemData)
{
	CheckEmpty(InItemName);
	CheckEmpty(InItemData.Desc);

	ItemName->SetText(FText::FromString(InItemName));
	Desc->SetText(FText::FromString(InItemData.Desc));
}
