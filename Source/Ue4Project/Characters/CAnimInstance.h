#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

UCLASS()
class UE4PROJECT_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
