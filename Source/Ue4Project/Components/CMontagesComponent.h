#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMontagesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;
		
};
