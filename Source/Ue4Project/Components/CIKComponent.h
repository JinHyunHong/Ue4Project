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
		FVector LeftDistance; // 왼발에 대한 왼발이 떨어져 있을때, 떨어진 곳과의 간격(X축만 사용)

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance; // 오른발과 떨어진 곳과의 간격(X축만 사용)

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftFootDistance; // 왼쪽 발 소켓과 충돌지점과의 정확한 거리

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightFootDistance; // 오른쪽 발 소켓과 충돌지점과의 정확한 거리

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance; // 허리(Z축만 사용)

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation; // 왼발에 대한 회전

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation; // 오른발에 대한 회전
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
		float TraceDistance = 55.0f; // 추적할 간격(거리)

	// TEnumAsByte : 자료형의 크기를 알 수 있도록(바이트 크기로 명시함)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float InterpSpeed = 17.0f; // 보간 속도

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
		float OffsetDistance = 5.0f; // 발과 땅사이에 보정간격 

public:
	FORCEINLINE const FIKData& GetData() { return Data; }


public:
	UCIKComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 발의 본과 지면의 붙은 곳의 거리를 구하는 내부함수
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation, float& OutFootDistance);

private:
	class ACharacter* OwnerCharacter;
	FIKData Data;
};
