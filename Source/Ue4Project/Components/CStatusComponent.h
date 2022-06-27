#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max,
};
// Sprint : 완전빠르게 달릴때


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4PROJECT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100.0f;

	// 에디터에서 속도 조정 가능
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 500, 700, 800 };

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint]; }

	FORCEINLINE bool CanMove() { return bCanMove; }

public:
	UCStatusComponent();

	void AddHealth(float InAmount);
	// Health를 줄이는 것
	void SubHealth(float InAmount);

	// 이동 가능하도록, 하지 못하도록 외부에서 세팅가능
	void SetMove();
	void SetStop();

	// MaxWalkSpeed에 해당 Speed모드의 값으로 세팅
	void SetSpeed(ECharacterSpeed InType);

protected:
	virtual void BeginPlay() override;

private:
	float Health;
	// 이동 가능한지
	bool bCanMove = true;

};
