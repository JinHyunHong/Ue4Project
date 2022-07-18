#include "Actions/CAction.h"
#include "Global.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"

UCAction::UCAction()
{
}

void UCAction::DestoryAction()
{
	if (!!Attachment)
	{
		if (Attachment->IsValidLowLevel())
			Attachment->Destroy();
	}
	
	if (!!Equipment)
	{
		if (Equipment->IsValidLowLevel())
			Equipment->Destroy();
	}

	if (!!DoAction)
	{
		if (DoAction->IsValidLowLevel())
			DoAction->Destroy();
	}

	MarkPendingKill();
}
