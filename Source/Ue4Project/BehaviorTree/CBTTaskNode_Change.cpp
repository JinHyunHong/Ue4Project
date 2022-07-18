#include "CBTTaskNode_Change.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"


UCBTTaskNode_Change::UCBTTaskNode_Change()
{
	bNotifyTick = true;

	NodeName = "Change";
}

EBTNodeResult::Type UCBTTaskNode_Change::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Change::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// OwnerComp : �̳��� ������ BehaviorTreeComponent
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	// ExcuteTask()���� ��带 �ٲپ��־���, Idle ��尡 �Ǿ��ٸ� ��尡 ���������� �ٲ���̴�.
	if (state->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
