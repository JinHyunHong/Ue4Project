#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CStatusComponent.h"
#include "CBTTaskNode_SetSightRadius.generated.h"

UCLASS()
class UE4PROJECT_API UCBTTaskNode_SetSightRadius : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float SightRadius = 600.0f;

	UPROPERTY(EditAnywhere)
		float LossSightRadius = 800.0f;

public:
	UCBTTaskNode_SetSightRadius();

protected:
	// Task가 실행될때 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
