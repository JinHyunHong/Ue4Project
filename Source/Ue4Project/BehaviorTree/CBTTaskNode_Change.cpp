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

	// OwnerComp : 이놈을 실행한 BehaviorTreeComponent
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	// ExcuteTask()에서 모드를 바꾸어주었고, Idle 모드가 되었다면 모드가 정상적으로 바뀐것이다.
	if (state->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
