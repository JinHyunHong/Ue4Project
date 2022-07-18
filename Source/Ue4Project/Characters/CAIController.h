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
		float BehaviorRange = 150; // 행동범위(액션을 하기 위한 범위)
	
	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true; // 디버그로 할지(디버그시에 구역을 보여줄 수 있도록 함)

	UPROPERTY(EditAnywhere)
		float AdjustCircleHeight = 50; // 디버깅용 원을 그릴때 높이를 조정할 값 


private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception; // 감지용 컴포넌트
	
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

public:
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }

public:
	ACAIController();

	float GetSightRadius(); // 감지시야설정하면 시야를 리턴

	// Actor에 Tick()을 재정의
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override; // 빙의 되면 호출됨
	virtual void OnUnPossess() override; //빙의가 끝날때 호출됨

private:
	// 감지가 되면 델리게이트에 연결될 함수
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdateActors);

public:
	FORCEINLINE class UAISenseConfig_Sight* GetSight() { return Sight; }

private:
	class ACEnemy_AI* OwnerEnemy; // 소유자
	// Perception 정의할때 시야로 감지하는데, 감지 정의하는 클래스
	class UAISenseConfig_Sight* Sight;


};
