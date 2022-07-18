#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4PROJECT_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float TraceRadius = 1000.0f; // 추적 반경

	UPROPERTY(EditAnywhere)
		float interopSpeed = 2.5f;

	// Trace Draw Debug 지속할 건지 	
	// 그래서 TEnumAsByte로 크기를 알려줌
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> Debug;

	// 타켓에 해골모양 파티클 붙이기 위한
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* Niagara;

public:
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 한번 누르면 타켓팅, 누르지 않으면 타켓팅이 풀림
	void ToggleTarget();
	void ChangeTargetLeft();
	void ChangeTargetRight();

public:
	void StartTargeting();
	void EndTargeting();

private:
	// 일정반경안에 있는지 보고 타켓을 잡을 수 있는 객체들을 찾아옴(BP와 똑같음)
	void SetTraceTargets();

	// 목록 중에 하나의 타겟을 정함
	void SetTarget();

	// 왼, 오른쪽 타겟으로 이동하기 위한 작업
	void ChangeTarget(bool InRight);

	// 타겟을 지정 + 거기에 커서를 붙임
	void ChangeCursor(class ACharacter* InTarget);

public:
	// 커서의 활성화 여부
	void ActiveCursor();
	void DeactiveCursor();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	ACharacter* GetTarget() const { return Target; }

private:
	// Player든, 적이든 타켓팅컴포넌트를 사용 가능하도록
	class ACharacter* OwnerCharacter;
	// 타켓팅될 캐릭터
	class ACharacter* Target;

	// Spawn되는 함수의 임시생성되는 컴포넌트를 저장(제거 안됨 방지)
	class UNiagaraComponent* Attached;

	// 추적할 수 있는 타켓 목록
	TArray<class ACharacter*> TraceTargets;
};
