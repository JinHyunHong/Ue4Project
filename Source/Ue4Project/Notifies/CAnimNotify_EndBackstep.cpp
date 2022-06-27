#include "Notifies/CAnimNotify_EndBackstep.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_EndBackstep::GetNotifyName_Implementation() const
{
	return "Backstep";
}

void UCAnimNotify_EndBackstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Backstep();
}
