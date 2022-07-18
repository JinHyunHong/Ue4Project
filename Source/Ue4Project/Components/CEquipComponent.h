#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Characters/CEquipCharacter.h"
#include "CEquipComponent.generated.h"

// FItem&�� �����Ϳ� ���� ������(Address)�� �ѱ�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEquipChanged, const EEquipmentType&, InEquipType, const FString&, ItemName, const FItem&, ItemData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCEquipComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		UDataTable* DataTable;

public:	
	UCEquipComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
		FEquipChanged OnEquipChanged; // Equip ������ �Ѱ���

	UPROPERTY(BlueprintAssignable)
		FEquipChanged OnUnequipChanged;

public:
	UFUNCTION(BlueprintCallable)
		void Equip(const EEquipmentType& InEquipType, const FString& InItemName);
	
	UFUNCTION(BlueprintCallable)
		void Unequip(const EEquipmentType& InEquipType);

public:
	void Equip(const EEquipmentType& InEquipType, const FString& InItemName, FItem& InItemData);
	void Unequip(const EEquipmentType& InEquipType, const FString& InItemName, FItem& InItemData);
};
