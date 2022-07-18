#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CActionComponent.h"
#include "CBTTaskNode_Change.generated.h"

UCLASS()
class UE4PROJECT_API UCBTTaskNode_Change : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		EActionType Type; // 어떤 무기로 바뀔지 지정
	
public:
	UCBTTaskNode_Change();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// 무기교체에 대한 시간이 필요하므로
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
