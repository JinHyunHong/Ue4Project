#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CEquipCharacter.h"
#include "Components/CStateComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class UE4PROJECT_API ACPlayer : public ACEquipCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveFoward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);


private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	
public:
	UFUNCTION()
		void OnEquipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);

	UFUNCTION()
		void OnUnequipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);

private:
	void OnRun();
	void OffRun();
	void OnAvoid();
	void OnViewEquipmnent();
	void OnViewInventory();
	void OnPickUp();

private:
	void OnDoAction();

public:
	void Begin_Roll();
	void Begin_Backstep();
	
	void End_Roll();
	void End_Backstep();

public:
	FORCEINLINE void SetInteractItem(AActor* InItem) { InteractItem = InItem; }


private:
	void Dead();

public:
	virtual void Begin_Dead() override;
	virtual void End_Dead() override;

private:
	class AActor* InteractItem;
};
