#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/CEquipCharacter.h"
#include "CInventoryComponent.generated.h"


// ���� ���� Visibillity ��Ȳ
UENUM(BlueprintType)
enum class EViewListType : uint8
{
	AllHidden, Equipment, Inventory, AllView, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	// Equipment, Iventory ���� ���ð����ϵ��� ����
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		TSubclassOf<class UCUserWidget_EquipInventory> EquipIventoryClass;

	// Equipment Widget���� ĳ���͸� �� �� �ֵ���
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		TSubclassOf<class ACEquipCharacter> EquipStateClass;

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void ViewAllHidden();
	
	UFUNCTION(BlueprintCallable)
		void ViewEquipment();
	
	UFUNCTION(BlueprintCallable)
		void ViewInventory();
	
	UFUNCTION(BlueprintCallable)
		void ViewAll();

public:
	UFUNCTION(BlueprintCallable)
		void AddItem(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData, bool bPickUp = false);
	
	UFUNCTION(BlueprintCallable)
		void AddEquip(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);

private:
	void OnEquipState(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData); // EquipState�� ��� ����
	void OnUnequipState(const EEquipmentType& InEquipType); // EquipState�� ��� ����

private:
	void SetViewList(const EViewListType& InType);


public:
	FORCEINLINE class UCUserWidget_Equipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCUserWidget_Inventory* GetInventory() { return Inventory; }

private:
	// �Ѱ��� �������� ���յǾ� ����
	class UCUserWidget_EquipInventory* EquipIventory;
	class UCUserWidget_Equipment* Equipment;
	class UCUserWidget_Inventory* Inventory;

private:
	class ACEquipCharacter* EquipState;
};
