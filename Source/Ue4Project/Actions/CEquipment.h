#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CAction.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegate);

UCLASS()
class UE4PROJECT_API ACEquipment : public AActor
{
	GENERATED_BODY()

public:
	// CActionData로 부터 값을 세팅함
	FORCEINLINE void SetEquipData(FEquipmentData InData) { EquipData = InData; }
	FORCEINLINE void SetUnequipData(FEquipmentData InData) { UnequipData = InData; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }

public:
	ACEquipment();

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	// 장착 개시(노티파이에 의해 콜 됨)
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	// 장착 완료
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	// 해제 명령(따로 장착해제 애니메이션 안할거여서 명령만 넣음)
	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Unequip();
	void Begin_Unequip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Unequip();
	void End_Unequip_Implementation();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	bool bEquipped;
	FEquipmentData EquipData;
	FEquipmentData UnequipData;
};
