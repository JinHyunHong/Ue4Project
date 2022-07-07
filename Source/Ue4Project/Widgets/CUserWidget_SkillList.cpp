#include "Widgets/CUserWidget_SkillList.h"
#include "Global.h"
#include "Widgets/CUserWidget_SkillSlot.h"
#include "GameFramework/PlayerController.h"
#include "Characters/CEquipCharacter.h"
#include "Components/GridPanel.h"

void UCUserWidget_SkillList::NativeConstruct()
{
	TArray<UWidget*> widgets = Grid->GetAllChildren();

	for (UWidget* widget : widgets)
	{
		UCUserWidget_SkillSlot* data = Cast<UCUserWidget_SkillSlot>(widget);
		if (!!data) Datas.Add(data);
	}

	Super::NativeConstruct();
}

UCUserWidget_SkillList::UCUserWidget_SkillList(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCUserWidget_SkillList::AddSkill(const FString& InName, class UTexture2D* InImage, const float& InCoolTime)
{
	// Index가 Slot개수보다 크다면 꽉찬 것으로 간주
	CheckTrue(Index > Grid->GetChildrenCount());

	Datas[Index]->Update(InName, InImage, InCoolTime);
	Index++;
}

void UCUserWidget_SkillList::ClearList()
{
	Index = 0;

	for (UCUserWidget_SkillSlot* data : Datas)
		data->Clear();
}

bool UCUserWidget_SkillList::IsValidSlot(uint32 InIndex)
{
	return Datas[InIndex]->GetSkillName() != "";
}


