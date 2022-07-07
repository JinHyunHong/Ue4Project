#include "Widgets/CUserWidget_SkillSlot.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCUserWidget_SkillSlot::NativePreConstruct()
{
	IndexText->SetText(FText::FromString(FString::FromInt(Index)));

	Super::NativePreConstruct();
}

void UCUserWidget_SkillSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCUserWidget_SkillSlot::Update(const FString& InSkillName, UTexture2D* InSkillImage, const float& InCoolTime)
{
	SkillImage->SetBrushFromTexture(InSkillImage);
	MaxCoolTime = InCoolTime;
	CoolTime = MaxCoolTime;
	SkillName = InSkillName;
	SkillBar->SetPercent(CoolTime / MaxCoolTime);
	bEnable = true;
}

void UCUserWidget_SkillSlot::Clear()
{
	SkillImage->SetBrushFromTexture(NULL);

	SkillName = "";
	bEnable = false;
	CoolTime = 0.0f;
	MaxCoolTime = 0.0f;
}

void UCUserWidget_SkillSlot::RestartCoolTime()
{
	CoolTime = 0.0f;
	SkillBar->SetPercent(CoolTime / MaxCoolTime);

	UKismetSystemLibrary::K2_SetTimer(this, "IncreaseCoolTime", 0.01f, true);
}

void UCUserWidget_SkillSlot::IncreaseCoolTime()
{
	CheckTrue(CoolTime >= MaxCoolTime);

	CoolTime += 0.01f;
	CoolTime = FMath::Clamp(CoolTime, 0.0f, MaxCoolTime);
	SkillBar->SetPercent(CoolTime / MaxCoolTime);

	CoolTime >= MaxCoolTime ? bEnable = true : bEnable = false;
}

