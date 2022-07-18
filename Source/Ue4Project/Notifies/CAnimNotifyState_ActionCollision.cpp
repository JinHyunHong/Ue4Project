#include "CAnimNotifyState_ActionCollision.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CAttachment.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_ActionCollision::GetNotifyName_Implementation() const
{
	return "ActionCollision";
}

void UCAnimNotifyState_ActionCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	ACAttachment* attachment = action->GetData()->GetAttachment();
	CheckNull(attachment);

	attachment->OnActionCollision();
}

void UCAnimNotifyState_ActionCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACAttachment* attachment = action->GetData()->GetAttachment();
	CheckNull(attachment);

	attachment->OffCollision();
}

