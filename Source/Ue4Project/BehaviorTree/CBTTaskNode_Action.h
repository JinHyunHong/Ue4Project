#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

UCLASS()
class UE4PROJECT_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()

private:
	// EditAnywhere �ٿ���� �ۿ� �׽�Ʈ �Ҷ� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = "AI")
		float MinDelay = 1.0f; // ���� ������ Min (�����ϰ� �ٽ� ���� ���� ������)

	UPROPERTY(EditAnywhere, Category = "AI")
		float MaxDelay = 2.0f; // ���� ������ Max (�����ϰ� �ٽ� ���� ���� ������)

public:
	UCBTTaskNode_Action();

protected:
	// Task�� ����ɶ�
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// Tick()�� �־����
	// ������ �Ϸ�� ���¿��� InProgress(Task ����)���¸� Tick()���� ���������
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float TotalTime = 0.0f; // ����ð�(���� �ð��� �帥 �� �����Ű�� ����)
	float Delay = 0.0f;
};
