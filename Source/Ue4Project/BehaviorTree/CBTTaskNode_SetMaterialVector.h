#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SetMaterialVector.generated.h"

UCLASS()
class UE4PROJECT_API UCBTTaskNode_SetMaterialVector : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_SetMaterialVector();

protected:
	// Task가 실행될때 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
