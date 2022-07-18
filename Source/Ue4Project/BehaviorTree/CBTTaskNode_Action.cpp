#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	// true�� ���־�� Tick�� ����� �� ����
	bNotifyTick = true;

	NodeName = "Action";
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);

	TotalTime = 0.0f;
	// ���� �����
	action->DoAction_Random();

	// ������ ���� Delay ����
	Delay = UKismetMathLibrary::RandomFloatInRange(MinDelay, MaxDelay);

	// InProgress(���) ���´� ���� �½�ũ�� ������� ���� ���·� ���� �½�ũ�� ��� �����ϰ� �ִ� ���°� ��
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);


	// ����� �ð��� ��
	TotalTime += DeltaSeconds;

	// idle ���� �ٲ���ٸ�(Action�� �����ٸ� Idle ���°� ��)
	// ����ð�(TotalTime)�� Delay���� ũ�ٸ� �½�ũ�� �Ϸ��(Finish) ������
	if (state->IsIdleMode() && TotalTime > Delay)
	{
		// FinishLatetAbort() : �ߴ����� ����
		// FinishLatentTask() : �����̳�, ���з� �½�ũ ����
		// Abort : �ߴܵ� �ɷ� ����, FinishLatetAbort()�� �����Ű�� ���� ����
		// InProgress(���)
		// Succeeded(�Ϸ�)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
