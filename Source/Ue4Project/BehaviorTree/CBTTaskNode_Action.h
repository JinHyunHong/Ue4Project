#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

UCLASS()
class UE4PROJECT_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()

private:
	// EditAnywhere 붙여줘야 밖에 테스트 할때 나타남
	UPROPERTY(EditAnywhere, Category = "AI")
		float MinDelay = 1.0f; // 공격 딜레이 Min (공격하고 다시 공격 들어갈때 딜레이)

	UPROPERTY(EditAnywhere, Category = "AI")
		float MaxDelay = 2.0f; // 공격 딜레이 Max (공격하고 다시 공격 들어갈때 딜레이)

public:
	UCBTTaskNode_Action();

protected:
	// Task가 실행될때
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// Tick()이 있어야함
	// 공격이 완료된 상태에서 InProgress(Task 중인)상태를 Tick()에서 종료시켜줌
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float TotalTime = 0.0f; // 경과시간(일정 시간이 흐른 후 종료시키기 위함)
	float Delay = 0.0f;
};
