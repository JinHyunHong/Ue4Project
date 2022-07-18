#include "CAnimNotify_Hitted.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

FString UCAnimNotify_Hitted::GetNotifyName_Implementation() const
{
	return "Hitted";
}

void UCAnimNotify_Hitted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(character);
	CheckNull(status);

	// IdleMode 상태 & 움직일 수 있는 상태
	state->SetIdleMode();
	status->SetMove();
	status->SetSpeed(ECharacterSpeed::Walk);
}

