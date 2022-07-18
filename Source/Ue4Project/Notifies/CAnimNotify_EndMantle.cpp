#include "Notifies/CAnimNotify_EndMantle.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndMantle::GetNotifyName_Implementation() const
{
	return "EndMantle";
}

void UCAnimNotify_EndMantle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	state->SetIdleMode();

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(character);
	CheckNull(parkour);

	// Flying -> Idle
	parkour->EndMantle();
}


