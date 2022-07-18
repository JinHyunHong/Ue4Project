#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class UE4PROJECT_API ACPatrolPath : public AActor
{
	GENERATED_BODY()

private:
	// 컨스트럭션 스크립트에서 다루기 위해서(C에서 컨스트럭션 스크립트를 다루는 방법이 있음)
	UPROPERTY(EditAnywhere, Category = "Loop")
		bool bLoop;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* Text;

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }
	
public:	
	ACPatrolPath();

	// BP의 컨스트럭트 스크립트(Construction Script)와 동일한 함수
	// 월드에서 변수들 속성 수정하면 인스턴스 각각 적용해서 처리할 수 있음
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

};
