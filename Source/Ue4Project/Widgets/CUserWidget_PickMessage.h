#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PickMessage.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_PickMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		float PlayRate = 0.15f;

protected:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* Visible;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	FORCEINLINE void SetForward(bool InForward = true) { bForward = InForward; }

private:
	bool bForward = true;
};
