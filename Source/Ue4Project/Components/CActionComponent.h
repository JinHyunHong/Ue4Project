#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CActionComponent.generated.h"


UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, OneHand, DoubleAxe, Max,
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
	void SetAction(const FString& InItemName);

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetData() { return Data; }

public:
	UFUNCTION(BlueprintPure)
		bool IsUnarmedMode();

	UFUNCTION(BlueprintPure)
		bool IsOneHandMode();

public:
	UCActionComponent();

	void OffAllCollision();
	void DestroyAllActions();

public:
	void DoAction();
	void DoSkill(const FString& InSkillName);
	void DoSkill(const int32& InIndex);

protected:
	virtual void BeginPlay() override;

private:
	void SetMode(EActionType InPrevType);
	FString GetLableName(class ACharacter* InOwnerCharacter, FString InName);


public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	class UCAction* Data;
};
