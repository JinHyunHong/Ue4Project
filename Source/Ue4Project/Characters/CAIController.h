#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class UE4PROJECT_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150; // �ൿ����(�׼��� �ϱ� ���� ����)
	
	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true; // ����׷� ����(����׽ÿ� ������ ������ �� �ֵ��� ��)

	UPROPERTY(EditAnywhere)
		float AdjustCircleHeight = 50; // ������ ���� �׸��� ���̸� ������ �� 


private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception; // ������ ������Ʈ
	
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

public:
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }

public:
	ACAIController();

	float GetSightRadius(); // �����þ߼����ϸ� �þ߸� ����

	// Actor�� Tick()�� ������
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override; // ���� �Ǹ� ȣ���
	virtual void OnUnPossess() override; //���ǰ� ������ ȣ���

private:
	// ������ �Ǹ� ��������Ʈ�� ����� �Լ�
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdateActors);

public:
	FORCEINLINE class UAISenseConfig_Sight* GetSight() { return Sight; }

private:
	class ACEnemy_AI* OwnerEnemy; // ������
	// Perception �����Ҷ� �þ߷� �����ϴµ�, ���� �����ϴ� Ŭ����
	class UAISenseConfig_Sight* Sight;


};
