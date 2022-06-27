#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CActionComponent.generated.h"


UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, OneHand, TwoHand, Max,
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
	void SetAction(const FString& InItemName);

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetData() { return Data; }

public:
	UFUNCTION(BlueprintPure)
		bool IsUnarmedMode();

	UFUNCTION(BlueprintPure)
		bool IsOneHandMode();

	UFUNCTION(BlueprintPure)
		bool IsTwoHandMode();

public:
	UCActionComponent();

	void OffAllCollision();
	void DestroyAllActions();

public:
	void DoAction();

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
