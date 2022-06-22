#include "Widgets/CUserWidget_Window.h"
#include "Global.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"


UCUserWidget_Window::UCUserWidget_Window(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

bool UCUserWidget_Window::Initialize()
{
	Super::Initialize();

	return true;
}

void UCUserWidget_Window::NativeConstruct()
{
	Super::NativeConstruct();

	ExitButton->OnPressed.AddDynamic(this, &UCUserWidget_Window::OnExitPress);
}

FReply UCUserWidget_Window::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (HasMouseCapture())
	{
		FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
		slot->SetPosition(mousePos - Offset);
		
		FEventReply reply = UWidgetBlueprintLibrary::Handled();
		return UWidgetBlueprintLibrary::CaptureMouse(reply, this).NativeReply;
	}

	return FReply::Handled();
}

FReply UCUserWidget_Window::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	FEventReply reply = UWidgetBlueprintLibrary::Handled();
	return UWidgetBlueprintLibrary::ReleaseMouseCapture(reply).NativeReply;
}

FReply UCUserWidget_Window::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	Offset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	FEventReply reply = UWidgetBlueprintLibrary::Handled();
	
	if (Offset.Y <= Panel->GetDesiredSize().Y)
		return UWidgetBlueprintLibrary::CaptureMouse(reply, this).NativeReply;
	
	return UWidgetBlueprintLibrary::ReleaseMouseCapture(reply).NativeReply;
}