#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class UE4PROJECT_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
	
private:
	// 수정가능하도록 EditDefaultsOnly
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree; // 운용할 BehaviorTree

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1; // TeamID : 이후에 피아를 구분하기 위한 값

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCPatrolComponent* Patrol;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

public:
	ACEnemy_AI();
};
