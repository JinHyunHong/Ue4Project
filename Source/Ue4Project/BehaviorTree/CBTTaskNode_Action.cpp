#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	// true로 해주어야 Tick이 실행될 수 있음
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
	// 공격 실행됨
	action->DoAction_Random();

	// 랜덤한 공격 Delay 생성
	Delay = UKismetMathLibrary::RandomFloatInRange(MinDelay, MaxDelay);

	// InProgress(대기) 상태는 현재 태스크가 종료되지 않은 상태로 현재 태스크를 계속 실행하고 있는 상태가 됨
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);


	// 경과한 시간이 됨
	TotalTime += DeltaSeconds;

	// idle 모드로 바뀌었다면(Action이 끝났다면 Idle 상태가 됨)
	// 경과시간(TotalTime)이 Delay보다 크다면 태스크가 완료된(Finish) 상태임
	if (state->IsIdleMode() && TotalTime > Delay)
	{
		// FinishLatetAbort() : 중단으로 종료
		// FinishLatentTask() : 성공이나, 실패로 태스크 종료
		// Abort : 중단된 걸로 종료, FinishLatetAbort()로 종료시키는 것이 좋음
		// InProgress(대기)
		// Succeeded(완료)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
