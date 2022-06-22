#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CEquipCharacter.h"
#include "CUserWidget_ToolTip.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_ToolTip : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Desc;

public:
	void Update(FString& InItemName, FItem& InItemData);
};
