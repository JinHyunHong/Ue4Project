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
		float TraceRadius = 1000.0f; // ���� �ݰ�

	UPROPERTY(EditAnywhere)
		float interopSpeed = 2.5f;

	// Trace Draw Debug ������ ���� 	
	// �׷��� TEnumAsByte�� ũ�⸦ �˷���
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> Debug;

	// Ÿ�Ͽ� �ذ��� ��ƼŬ ���̱� ����
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* Niagara;

public:
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:
	// �ѹ� ������ Ÿ����, ������ ������ Ÿ������ Ǯ��
	void ToggleTarget();
	void ChangeTargetLeft();
	void ChangeTargetRight();

public:
	void StartTargeting();
	void EndTargeting();

private:
	// �����ݰ�ȿ� �ִ��� ���� Ÿ���� ���� �� �ִ� ��ü���� ã�ƿ�(BP�� �Ȱ���)
	void SetTraceTargets();

	// ��� �߿� �ϳ��� Ÿ���� ����
	void SetTarget();

	// ��, ������ Ÿ������ �̵��ϱ� ���� �۾�
	void ChangeTarget(bool InRight);

	// Ÿ���� ���� + �ű⿡ Ŀ���� ����
	void ChangeCursor(class ACharacter* InTarget);

public:
	// Ŀ���� Ȱ��ȭ ����
	void ActiveCursor();
	void DeactiveCursor();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	ACharacter* GetTarget() const { return Target; }

private:
	// Player��, ���̵� Ÿ����������Ʈ�� ��� �����ϵ���
	class ACharacter* OwnerCharacter;
	// Ÿ���õ� ĳ����
	class ACharacter* Target;

	// Spawn�Ǵ� �Լ��� �ӽû����Ǵ� ������Ʈ�� ����(���� �ȵ� ����)
	class UNiagaraComponent* Attached;

	// ������ �� �ִ� Ÿ�� ���
	TArray<class ACharacter*> TraceTargets;
};
