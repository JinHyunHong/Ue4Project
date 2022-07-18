#include "CAnimNotifyState_SkillCollision.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CAttachment.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_SkillCollision::GetNotifyName_Implementation() const
{
	return "SkillCollision";
}

void UCAnimNotifyState_SkillCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	ACAttachment* attachment = action->GetData()->GetAttachment();
	CheckNull(attachment);

	attachment->OnSkillCollision(Index);
}

void UCAnimNotifyState_SkillCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

