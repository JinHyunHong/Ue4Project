#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"


UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Avoid, Angry,

};

// Wait : 대기 상태
// Approach : 추격 상태
// Action : 액션 상태
// Patrol : 순찰 상태
// Hitted : 공격 받은 상태
// Avoid : 회피 상태(원거리 공격 떄 나옴)
// Angry : 폭주 상태(보스에 경우 필요)

// 상태가 변경되었을 때, 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4PROJECT_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// 모든 AI가 블랙보드를 공유해서 쓸 것인데, 그때 블랙보드 키가 됨
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior"; // Behavior 블랙보드에 키를 넣을 것임

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player"; // 플레이어 키

public:
	// 블루프린트에서 콜 할수 있도록, 리턴 받을 수 있도록
	UFUNCTION(BlueprintPure)
		bool IsWaitMode();

	UFUNCTION(BlueprintPure)
		bool IsApproachMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsPatrolMode();

	UFUNCTION(BlueprintPure)
		bool IsHittedMode();

	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

	UFUNCTION(BlueprintPure)
		bool IsAngryMode();

	// 외부에서 블랙보드 세팅가능
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	UCBehaviorComponent();

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetAngryMode();


	// 가지고 있는 타겟 리턴
	class ACPlayer* GetTargetPlayer();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType(); // 현재 타입 리턴

public:
	// BP에서 할당 가능하도록
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
};
