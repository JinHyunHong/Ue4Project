#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	action->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());

	IsInAir = character->GetMovementComponent()->IsFalling();
	IsAccelerating = character->GetCharacterMovement()->GetCurrentAcceleration().Size2D() > 0;

	FRotator rotation = TryGetPawnOwner()->GetBaseAimRotation();
	Yaw = rotation.Yaw;
	Pitch = rotation.Pitch;

	UCIKComponent* ik = CHelpers::GetComponent<UCIKComponent>(character);
	if (!!ik)
	{
		IKData = ik->GetData();
		ACPlayer* player = Cast<ACPlayer>(character);
		float distance = player->GetDeduceDistance();

		// IK를 이용해서 FootSound Play
		if (IKData.LeftFootDistance.X <= distance || IKData.RightFootDistance.X <= distance)
			player->PlayFootstep();
	}
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}