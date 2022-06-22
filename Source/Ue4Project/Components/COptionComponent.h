#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		float HorizontalLookRate = 45;

	UPROPERTY(EditDefaultsOnly)
		float VerticalLookRate = 45;

public:
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

public:
	UCOptionComponent();

protected:
	virtual void BeginPlay() override;
};
