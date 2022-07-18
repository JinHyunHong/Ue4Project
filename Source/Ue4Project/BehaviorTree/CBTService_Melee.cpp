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
	// NodeName�� �̸��� �������ָ� BehaviorTree�� �̸��� ��Ÿ��
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorTreeComponent�� �ִ� GetOwner�� ���ָ� ��
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	// AIController�� Actor�� ���� ��ӹ޾Ƽ� GetComponent����
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	// ���ǵǾ��ִ� Pawn�������� ��
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	// owner�� Boss�̰�, HealthPercent�� AngryPercent ���� ���� ��� ���� ����
	if (ai->IsBoss() && status->GetHealthPercent() < AngryPercent)
	{
		behavior->SetAngryMode();

		return;
	}

	// ���� �ǰݻ��¶��
	if (state->IsHittedMode() || state->IsDeadMode())
	{
		behavior->SetHittedMode();

		return;
	}

	ACPlayer* target = behavior->GetTargetPlayer();
	// Ÿ���� ���ٸ� wait���
	if (target == NULL)
	{
		bool bPatrol = true;
		bPatrol &= patrol != NULL;
		bPatrol &= patrol->IsValid();
		bPatrol &= status->CanMove();

		if (bPatrol)
		{
			// patrol�� �ְ�, Path�� Null�� �ƴ϶��, ������ �� �ִ� ���¶��
			behavior->SetPatrolMode();

			return;
		}

		behavior->SetWaitMode();

		return;
	}
	else
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		// Ÿ���� �ִµ� ���� ���¶��
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();

			return;
		}
	}

	// ���� ������ �Ÿ��� �ִ���
	// ai�� target�� �Ÿ��� ����
	float distance = ai->GetDistanceTo(target);

	if (distance < controller->GetBehaviorRange())
	{
		// ����
		behavior->SetActionMode();

		return;
	}

	// ���� ���� �ȿ� �ִٸ�, ������ �� �ִ� ���¶��
	if (distance < controller->GetSightRadius() && status->CanMove())
	{
		// �������� ����
		behavior->SetApproachMode();

		return;
	}
}