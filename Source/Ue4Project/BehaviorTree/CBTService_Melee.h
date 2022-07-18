#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class UE4PROJECT_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Melee();

private:
	UPROPERTY(EditAnywhere, Category = "AI")
		float AngryPercent = 0.3f; // Angry 기준 Percent;

protected:
	// 매프레임마다 호출
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
