#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Actions/CAction.h"
#include "Actions/CDoAction.h"
#include "Components/CActionComponent.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	// 애초에 없으면 이 애니메이션에 들어올 수 없어서 따로 NULL체크 안함
	action->GetData()->GetDoAction()->End_DoAction();
}


