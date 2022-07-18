#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class ACPatrolPath* Path;

	UPROPERTY(EditAnywhere)
		int32 Index; // ���� ����� Ÿ�� �ִ���

	UPROPERTY(EditAnywhere)
		bool bReverse; // ������ ������(������ �����Ұ���)

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 50; // ���޹ݰ�(�󸶸�ŭ �ٹ����� ������ ���޷� ��������)
	
public:
	// �ִ���
	FORCEINLINE bool IsValid() { return Path != NULL; }

	// ���� �̵�����(�����ġ�� �̵�����, �Ÿ��� �󸶳� �Ǵ���) 
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);

	// ���������� �������� �Ѱ����
	void UpdateNextIndex();

public:	
	UCPatrolComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void SetPath(class ACPatrolPath* InPath) { Path = InPath; }

	UFUNCTION(BlueprintCallable)
		void SetIndex(const int32& InIndex) { Index = InIndex; }

	UFUNCTION(BlueprintCallable)
		void SetReverse(bool InReverse) { bReverse = InReverse; }

	UFUNCTION(BlueprintCallable)
		void RandomIndex();

	UFUNCTION(BlueprintPure)
		int32 GetNumberOfSplinePoints();

	UFUNCTION(BlueprintPure)
		int32& GetIndex() { return Index; }

	UFUNCTION(BlueprintPure)
		bool GetReverse() { return bReverse; }

public:
	FORCEINLINE void Active() { bActive = true; }
	FORCEINLINE void Inactive() { bActive = false; }

private:
	bool bActive = false;
};
