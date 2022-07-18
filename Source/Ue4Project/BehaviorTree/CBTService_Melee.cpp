#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CPatrolComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	// NodeName에 이름을 세팅해주면 BehaviorTree에 이름이 나타남
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorTreeComponent에 있는 GetOwner를 해주면 됨
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	// AIController도 Actor로 부터 상속받아서 GetComponent가능
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	// 빙의되어있는 Pawn가져오면 됨
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	// owner가 Boss이고, HealthPercent가 AngryPercent 보다 작은 경우 폭주 상태
	if (ai->IsBoss() && status->GetHealthPercent() < AngryPercent)
	{
		behavior->SetAngryMode();

		return;
	}

	// 내가 피격상태라면
	if (state->IsHittedMode() || state->IsDeadMode())
	{
		behavior->SetHittedMode();

		return;
	}

	ACPlayer* target = behavior->GetTargetPlayer();
	// 타겟이 없다면 wait모드
	if (target == NULL)
	{
		bool bPatrol = true;
		bPatrol &= patrol != NULL;
		bPatrol &= patrol->IsValid();
		bPatrol &= status->CanMove();

		if (bPatrol)
		{
			// patrol이 있고, Path가 Null이 아니라면, 움직일 수 있는 상태라면
			behavior->SetPatrolMode();

			return;
		}

		behavior->SetWaitMode();

		return;
	}
	else
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		// 타겟이 있는데 죽은 상태라면
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();

			return;
		}
	}

	// 공격 가능한 거리에 있는지
	// ai와 target의 거리를 구함
	float distance = ai->GetDistanceTo(target);

	if (distance < controller->GetBehaviorRange())
	{
		// 공격
		behavior->SetActionMode();

		return;
	}

	// 감지 범위 안에 있다면, 움직일 수 있는 상태라면
	if (distance < controller->GetSightRadius() && status->CanMove())
	{
		// 추적모드로 변경
		behavior->SetApproachMode();

		return;
	}
}