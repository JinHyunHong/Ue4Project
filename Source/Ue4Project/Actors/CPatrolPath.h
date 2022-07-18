#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class UE4PROJECT_API ACPatrolPath : public AActor
{
	GENERATED_BODY()

private:
	// ����Ʈ���� ��ũ��Ʈ���� �ٷ�� ���ؼ�(C���� ����Ʈ���� ��ũ��Ʈ�� �ٷ�� ����� ����)
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

	// BP�� ����Ʈ��Ʈ ��ũ��Ʈ(Construction Script)�� ������ �Լ�
	// ���忡�� ������ �Ӽ� �����ϸ� �ν��Ͻ� ���� �����ؼ� ó���� �� ����
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

};
