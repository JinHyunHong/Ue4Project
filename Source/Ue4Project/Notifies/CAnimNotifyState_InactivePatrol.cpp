#include "Notifies/CAnimNotifyState_InactivePatrol.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CStatusComponent.h"
#include "Components/CPatrolComponent.h"

FString UCAnimNotifyState_InactivePatrol::GetNotifyName_Implementation() const
{
	return "InactivePatrol";
}

void UCAnimNotifyState_InactivePatrol::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(MeshComp->GetOwner());
	CheckNull(status);

	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(MeshComp->GetOwner());
	CheckNull(patrol);

	status->SetStop();
	patrol->Inactive();
}

void UCAnimNotifyState_InactivePatrol::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(MeshComp->GetOwner());
	CheckNull(status);

	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(MeshComp->GetOwner());
	CheckNull(patrol);

	status->SetMove();
	patrol->Active();
}

