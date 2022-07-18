#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CActionComponent.generated.h"


UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, OneHand, DoubleAxe, Enemy, Max,
};


// ������ �ٲ������ �ൿó���� ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		UDataTable* DataTable;

public:
	UFUNCTION(BlueprintCallable)
		void SetUnarmedMode();

	UFUNCTION(BlueprintCallable)
		void SetOneHandMode();

	UFUNCTION(BlueprintCallable)
		void SetDoubleAxeMode();

	UFUNCTION(BlueprintCallable)
		void SetEnemyMode();

public:
	UFUNCTION(BlueprintPure)
		bool IsUnarmedMode();

	UFUNCTION(BlueprintPure)
		bool IsOneHandMode();

	UFUNCTION(BlueprintPure)
		bool IsDoubleAxeMode();

	UFUNCTION(BlueprintPure)
		bool IsEnemyMode();

public:
	UCActionComponent();

	void OffAllCollision();
	void DestroyAllActions();

public:
	void DoAction();
	void DoAction_Random();
	
	void DoSkill(const FString& InSkillName);
	void DoSkill(const int32& InIndex);
	void DoSkill_Random();

protected:
	virtual void BeginPlay() override;

public:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InType);

private:
	FString GetLableName(class ACharacter* InOwnerCharacter, FString InName);

public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;
	
private:
	// ������ ���̺��� ���� ��ü ����
	void SetData(const EActionType& InType, class UCAction** OutData);

public:
	class UCAction* GetPrevData() { return Datas[(int32)PrevType]; }
	class UCAction* GetData() { return Datas[(int32)Type]; }

private:
	EActionType PrevType; // ���� Ÿ��
	EActionType Type;

private:
	// Unreal Reflection �ý����� UObject�� ���� ������ ī��Ʈ�� ������ �� �ֵ��� �ϱ� ���� UObject �Ļ� �����͸� ��ũ��
	// �̷��� �ϸ�, �߰��� �����Ͱ� �սǵ��� ����
	UPROPERTY()
		class UCAction* Datas[(int32)EActionType::Max];
};
