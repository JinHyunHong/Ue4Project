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

	// 해당 스피드타입에 맞도록 캐릭터의 MaxWalkSpeed 세팅
	status->SetSpeed(SpeedType);

	// Task는 종료가 중요(종료안하면 대기상태임)
	// EBTNodeResult
	// Succeeded, Faild, InProgress(현재 작업이 완료되지 않은 상태)가 있음
	// 리턴을 안하면 InProgress 상태가 됨
	
	// BP에서는 InProgress가 따로 없음, 상태를 안주면 InProgress 상태를 유지함
	// C에서는 InProgress로 해서 공격이 완료되었을 때 Success를 시키거나 해야함 
	return EBTNodeResult::Succeeded;
}


