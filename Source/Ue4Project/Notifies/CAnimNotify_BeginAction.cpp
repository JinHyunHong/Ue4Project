#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Actions/CAction.h"
#include "Actions/CDoAction.h"
#include "Components/CActionComponent.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	UCAction* actionData = action->GetData();
	CheckNull(actionData);

	ACDoAction* doAction = actionData->GetDoAction();
	CheckNull(doAction);

	doAction->Begin_DoAction();
}


