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


// 장착이 바뀌었을때 행동처리를 위해
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
	// 데이터 테이블을 통해 객체 생성
	void SetData(const EActionType& InType, class UCAction** OutData);

public:
	class UCAction* GetPrevData() { return Datas[(int32)PrevType]; }
	class UCAction* GetData() { return Datas[(int32)Type]; }

private:
	EActionType PrevType; // 이전 타입
	EActionType Type;

private:
	// Unreal Reflection 시스템이 UObject에 대한 포인터 카운트를 참조할 수 있도록 하기 위해 UObject 파생 포인터를 마크업
	// 이렇게 하면, 중간에 데이터가 손실되지 않음
	UPROPERTY()
		class UCAction* Datas[(int32)EActionType::Max];
};
