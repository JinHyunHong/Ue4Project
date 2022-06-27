#include "CAnimNotifyState_Dead.h"
#include "Global.h"
#include "Characters/ICharacter.h"

FString UCAnimNotifyState_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UCAnimNotifyState_Dead::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->Begin_Dead();
}

void UCAnimNotifyState_Dead::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->End_Dead();
}


