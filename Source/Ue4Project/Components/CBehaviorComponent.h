#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"


UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Avoid, Angry,

};

// Wait : ��� ����
// Approach : �߰� ����
// Action : �׼� ����
// Patrol : ���� ����
// Hitted : ���� ���� ����
// Avoid : ȸ�� ����(���Ÿ� ���� �� ����)
// Angry : ���� ����(������ ��� �ʿ�)

// ���°� ����Ǿ��� ��, ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4PROJECT_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// ��� AI�� �����带 �����ؼ� �� ���ε�, �׶� ������ Ű�� ��
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior"; // Behavior �����忡 Ű�� ���� ����

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player"; // �÷��̾� Ű

public:
	// �������Ʈ���� �� �Ҽ� �ֵ���, ���� ���� �� �ֵ���
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

	// �ܺο��� ������ ���ð���
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


	// ������ �ִ� Ÿ�� ����
	class ACPlayer* GetTargetPlayer();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType(); // ���� Ÿ�� ����

public:
	// BP���� �Ҵ� �����ϵ���
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
};
