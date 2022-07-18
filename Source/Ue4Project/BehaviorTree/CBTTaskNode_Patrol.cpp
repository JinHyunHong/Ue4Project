#include "CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CPatrolComponent.h"
#include "Components/CStateComponent.h"


UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	bNotifyTick = true;

	NodeName = "Patrol";
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
	
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNullResult(state, EBTNodeResult::Failed);

	// DeadMode��� Patrol�� �������� ����
	CheckTrueResult(state->IsDeadMode(), EBTNodeResult::Failed);

	FVector location; // ��� ��ġ�� �̵��Ұ���
	float acceptance; // ���޹ݰ�

	// �� �� ���� ���̶�� Failed ����
	if (patrol->GetMoveTo(location, acceptance) == false)
		return EBTNodeResult::Failed;

	// �����Ҷ� ������ InProgress�̴�.(InProgress�� �ݵ�� �ϷḦ ������� ��)
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);;

	FVector location; // ��� ��ġ�� �̵��Ұ���
	float acceptance; // ���޹ݰ�

	EPathFollowingRequestResult::Type type = EPathFollowingRequestResult::Failed;

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	
	if (state == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (state->IsDeadMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (patrol->GetMoveTo(location, acceptance)) type = controller->MoveToLocation(location, acceptance, false);
	// MoveToLocation() : AI Controller�� �ش� ��ġ�� �̵���Ű�� �Լ�
	// AIMoveTo �Լ��� ���������� �� �Լ��� ȣ���� �̵��ϰ� �ȴ�.
	// �� �Լ��� �����Ҽ� �ִ���, ���� �ִ����� ��������
	// 1. ���� ����, 2. �󸶸�ŭ ������ ���޷� ��������, 3 : �ΰ��� �������� ������ ����, 4 bUsePathFinding : �츮�� �׺���̼� �޽�����ϴϱ� �⺻������ true�� �ش�.
	// ��Ÿ ���� bCanStrafe : �������� ĥ����

	/*
	UENUM(BlueprintType)
	namespace EPathFollowingRequestResult
	{
		enum Type
		{
			Failed, // �� �� ���� ��ġ
			AlreadyAtGoal, // ���� �����ߴٸ�
			RequestSuccessful // ���� ������ ���������� ���ᰡ �Ǿ���(�� �� �ִ� ��ġ������ Ȯ���Ҷ� ���)
		};
	}
	*/

	if (type == EPathFollowingRequestResult::Failed)
	{
		// �� �� ���ٸ� Failed�� ����
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}
	
	// ���� �����ߴٸ�
	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		patrol->UpdateNextIndex();
	}
}