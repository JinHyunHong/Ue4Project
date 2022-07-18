#include "CBTTaskNode_SetSpeed.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_SetSpeed::UCBTTaskNode_SetSpeed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);

	// �ش� ���ǵ�Ÿ�Կ� �µ��� ĳ������ MaxWalkSpeed ����
	status->SetSpeed(SpeedType);

	// Task�� ���ᰡ �߿�(������ϸ� ��������)
	// EBTNodeResult
	// Succeeded, Faild, InProgress(���� �۾��� �Ϸ���� ���� ����)�� ����
	// ������ ���ϸ� InProgress ���°� ��
	
	// BP������ InProgress�� ���� ����, ���¸� ���ָ� InProgress ���¸� ������
	// C������ InProgress�� �ؼ� ������ �Ϸ�Ǿ��� �� Success�� ��Ű�ų� �ؾ��� 
	return EBTNodeResult::Succeeded;
}


