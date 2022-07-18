#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class UE4PROJECT_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
	
private:
	// ���������ϵ��� EditDefaultsOnly
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree; // ����� BehaviorTree

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1; // TeamID : ���Ŀ� �ǾƸ� �����ϱ� ���� ��

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCPatrolComponent* Patrol;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

public:
	ACEnemy_AI();
};
