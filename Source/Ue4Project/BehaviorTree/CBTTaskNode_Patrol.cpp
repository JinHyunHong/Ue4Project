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

	// DeadMode라면 Patrol을 수행하지 않음
	CheckTrueResult(state->IsDeadMode(), EBTNodeResult::Failed);

	FVector location; // 어느 위치로 이동할건지
	float acceptance; // 도달반경

	// 갈 수 없는 곳이라면 Failed 상태
	if (patrol->GetMoveTo(location, acceptance) == false)
		return EBTNodeResult::Failed;

	// 도달할때 까지가 InProgress이다.(InProgress면 반드시 완료를 시켜줘야 함)
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);;

	FVector location; // 어느 위치로 이동할건지
	float acceptance; // 도달반경

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
	// MoveToLocation() : AI Controller를 해당 위치로 이동시키는 함수
	// AIMoveTo 함수가 내부적으로 이 함수를 호출해 이동하게 된다.
	// 이 함수가 도달할수 있는지, 가고 있는지를 리턴해줌
	// 1. 어디로 갈지, 2. 얼마만큼 갔을때 도달로 간주할지, 3 : 두개가 겹쳤을때 정지할 건지, 4 bUsePathFinding : 우리는 네비게이션 메쉬사용하니까 기본적으로 true로 준다.
	// 기타 인자 bCanStrafe : 옆걸음질 칠건지

	/*
	UENUM(BlueprintType)
	namespace EPathFollowingRequestResult
	{
		enum Type
		{
			Failed, // 갈 수 없는 위치
			AlreadyAtGoal, // 현재 도달했다면
			RequestSuccessful // 현재 완전히 정상적으로 종료가 되었다(갈 수 있는 위치인지를 확인할때 사용)
		};
	}
	*/

	if (type == EPathFollowingRequestResult::Failed)
	{
		// 갈 수 없다면 Failed로 해줌
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}
	
	// 현재 도달했다면
	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		patrol->UpdateNextIndex();
	}
}