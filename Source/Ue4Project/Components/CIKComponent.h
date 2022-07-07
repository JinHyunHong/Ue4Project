#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CIKComponent.generated.h"


USTRUCT(BlueprintType)
struct FIKData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance; // �޹߿� ���� �޹��� ������ ������, ������ ������ ����(X�ุ ���)

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance; // �����߰� ������ ������ ����(X�ุ ���)

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftFootDistance; // ���� �� ���ϰ� �浹�������� ��Ȯ�� �Ÿ�

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightFootDistance; // ������ �� ���ϰ� �浹�������� ��Ȯ�� �Ÿ�

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance; // �㸮(Z�ุ ���)

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation; // �޹߿� ���� ȸ��

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation; // �����߿� ���� ȸ��
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCIKComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		FName LeftSocket = "foot_l";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		FName RightSocket = "foot_r";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float TraceDistance = 55.0f; // ������ ����(�Ÿ�)

	// TEnumAsByte : �ڷ����� ũ�⸦ �� �� �ֵ���(����Ʈ ũ��� �����)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float InterpSpeed = 17.0f; // ���� �ӵ�

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float OffsetDistance = 5.0f; // �߰� �����̿� �������� 

public:
	FORCEINLINE const FIKData& GetData() { return Data; }


public:
	UCIKComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// ���� ���� ������ ���� ���� �Ÿ��� ���ϴ� �����Լ�
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation, float& OutFootDistance);

private:
	class ACharacter* OwnerCharacter;
	FIKData Data;
};
