#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CStatusComponent.h"
#include "CBTTaskNode_SetSpeed.generated.h"

UCLASS()
class UE4PROJECT_API UCBTTaskNode_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		ECharacterSpeed SpeedType;

public:
	UCBTTaskNode_SetSpeed();

protected:
	// Task가 실행될때 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
