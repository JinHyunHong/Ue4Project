#include "CBTTaskNode_Skill.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Skill::UCBTTaskNode_Skill()
{
	// true�� ���־�� Tick�� ����� �� ����
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
	// ���� �����
	action->DoSkill_Random();

	// ������ ���� Delay ����
	Delay = UKismetMathLibrary::RandomFloatInRange(MinDelay, MaxDelay);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
