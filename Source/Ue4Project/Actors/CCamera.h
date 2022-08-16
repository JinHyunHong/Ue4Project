#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CCamera.generated.h"

UCLASS()
class UE4PROJECT_API ACCamera : public ACameraActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		bool bEnable; // ��밡������ ����

	UPROPERTY(EditAnywhere)
		float Width; // P0, P3 �׸��� P1, P2 ���� ����

	UPROPERTY(EditAnywhere)
		float Height; // P0, P1 �׸��� P2, P3 ���� ����
	
	UPROPERTY(EditAnywhere)
		float Duration = 3.0f; // ���

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void Enable() { bEnable = true; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsEnable() { return bEnable; }

private:
	float Time; // ���� �ð�
	FVector StartPos; // P0
	FVector StartHeightPos; // P1
	FVector EndPos; // P3
	FVector EndHeightPos; // P2
};
