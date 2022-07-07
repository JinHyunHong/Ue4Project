#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillList.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_SkillList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* Grid;

public:
	UCUserWidget_SkillList(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void AddSkill(const FString& InName, class UTexture2D* InImage, const float& InCoolTime);
	void ClearList();

public:
	FORCEINLINE class UCUserWidget_SkillSlot* GetSlot(uint32 InIndex) { return Datas[InIndex]; }
	bool IsValidSlot(uint32 InIndex);

protected:
	int32 Index = 0;
	TArray<class UCUserWidget_SkillSlot*> Datas;
	
};
