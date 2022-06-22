#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Window.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_Window : public UUserWidget
{
	GENERATED_BODY()

public:
	UCUserWidget_Window(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* Panel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ExitButton;

protected:
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


protected:
	UFUNCTION(BlueprintCallable, Category = "ExitButton")
		virtual void OnExitPress() {};

private:
	FVector2D Offset;
};
