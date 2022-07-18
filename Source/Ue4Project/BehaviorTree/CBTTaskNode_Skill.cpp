#include "CBTTaskNode_Skill.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Skill::UCBTTaskNode_Skill()
{
	// true로 해주어야 Tick이 실행될 수 있음
	bNotifyTick = true;

	NodeName = "Skill";
}

EBTNodeResult::Type UCBTTaskNode_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);

	TotalTime = 0.0f;
	// 공격 실행됨
	action->DoSkill_Random();

	// 랜덤한 공격 Delay 생성
	Delay = UKismetMathLibrary::RandomFloatInRange(MinDelay, MaxDelay);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
