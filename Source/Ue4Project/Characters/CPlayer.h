#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CEquipCharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class UE4PROJECT_API ACPlayer : public ACEquipCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

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

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveFoward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

private:
	void OnViewEquipmnent();
	void OnViewInventory();
	void OnPickUp();

public:
	FORCEINLINE void SetInteractItem(AActor* InItem) { InteractItem = InItem; }

private:
	class AActor* InteractItem;
};
