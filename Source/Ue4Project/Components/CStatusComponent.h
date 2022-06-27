#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max,
};
// Sprint : ���������� �޸���


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4PROJECT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100.0f;

	// �����Ϳ��� �ӵ� ���� ����
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
	// Health�� ���̴� ��
	void SubHealth(float InAmount);

	// �̵� �����ϵ���, ���� ���ϵ��� �ܺο��� ���ð���
	void SetMove();
	void SetStop();

	// MaxWalkSpeed�� �ش� Speed����� ������ ����
	void SetSpeed(ECharacterSpeed InType);

protected:
	virtual void BeginPlay() override;

private:
	float Health;
	// �̵� ��������
	bool bCanMove = true;

};
