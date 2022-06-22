#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool CanMove() { return bCanMove; }

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bCanMove = true;
};
