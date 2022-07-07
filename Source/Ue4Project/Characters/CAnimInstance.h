#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "Components/CIKComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class UE4PROJECT_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsInAir; // Falling 상태인지

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsAccelerating; // 가속도가 0이상인지

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Yaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FIKData IKData; // IK에 관한 정보(간격에 관한 데이터가 들어가 있음)
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);
};
