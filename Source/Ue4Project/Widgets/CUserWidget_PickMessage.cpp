#include "Widgets/CUserWidget_PickMessage.h"
#include "Global.h"

void UCUserWidget_PickMessage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckTrue(Visibility != ESlateVisibility::Visible);
	CheckTrue(IsAnimationPlaying(Visible));
	
	if (bForward)
	{
		PlayAnimationReverse(Visible, PlayRate);
		bForward = false;

		return;
	}

	PlayAnimationForward(Visible, PlayRate);
	bForward = true;
}