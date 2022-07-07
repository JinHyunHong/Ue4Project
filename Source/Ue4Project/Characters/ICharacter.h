#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class UE4PROJECT_API IICharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ChangeColor(FLinearColor InColor); // 캐릭터 마다 Material수가 다름
	virtual void ChangeColor_Implementation(FLinearColor InColor) {};

	virtual void Begin_Dead() {};
	virtual void End_Dead() {};
};
